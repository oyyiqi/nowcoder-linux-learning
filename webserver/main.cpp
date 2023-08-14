#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <assert.h>

#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"
using namespace std;

#define MAX_FD 65536
#define MAX_EVENT_NUMBER 10000
#define IP INADDR_ANY
#define PORT 8888
#define BACKLOG 5

extern int addfd(int epollfd, int fd, bool one_shot);
extern int removefd(int epollfd, int fd);

void addsig(int sig, void(handler)(int), bool restart = true)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    if (restart)
    {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    sa.sa_handler = handler;
    assert(sigaction(sig, &sa, NULL) != -1);
}

void show_error(int connfd, const char *info)
{
    printf("%s", info);
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int main()
{
    addsig(SIGPIPE, SIG_IGN); // 忽略SIGPIPE信号
    // 当一个进程在网络通信中向一个已关闭的连接写数据时，会触发 SIGPIPE 信号。
    // 这可以防止进程因为试图向一个不再可用的连接写数据而导致崩溃

    threadpool<http_conn> *pool = NULL;
    try
    {
        pool = new threadpool<http_conn>; // 等价于 new threadpool<http_conn>()
    }
    catch (const std::exception &e)
    {
        return 1;
    }

    http_conn* users = new http_conn[MAX_FD];
    assert(users);
    int user_count = 0;

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    /*设置套接字关闭时是否立即关闭，还是等待一段时间*/
    struct linger tmp = {1, 0};  // 1:启用SO_LINGER, 0:等待时间
    setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));

    int ret = 0;
    struct sockaddr_in addr;
    memset(&addr, '\0', sizeof(addr));
    addr.sin_addr.s_addr = IP;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    ret = bind(listenfd, (sockaddr *)&addr, sizeof(addr));
    assert(ret >= 0);

    ret = listen(listenfd, BACKLOG);
    assert(ret >= 0);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(1);
    assert(epollfd != -1);
    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;
    
    while(true)
    {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if((number < 0) && errno != EINTR)
        {
            printf("epoll failure\n");
            break;
        }

        for(int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd)
            {
                sockaddr_in client_addr;
                socklen_t sock_len = sizeof(client_addr);
                int connfd = accept(sockfd, (sockaddr *)&client_addr, &sock_len);
                char client_ip[16] = "";
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
                unsigned short port = ntohs(client_addr.sin_port);
                cout << "[CONNECT] ----> " << client_ip << ":" << port << endl;

                if(connfd < 0)
                {
                    perror("accept");
                    continue;
                }
                if(http_conn::m_user_count >= MAX_FD)
                {
                    show_error(connfd, "Internal server busy");
                    continue;
                }
                users[connfd].init_conn(connfd, client_addr);
            }

            /*如果客户端关闭连接或者连接出错，释放资源*/
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                users[sockfd].close_conn();
            }
            else if(events[i].events & EPOLLIN)
            {
                if(users[sockfd].read())
                {
                    pool->append(users + sockfd);
                }
                else
                {
                    users[sockfd].close_conn();
                }
            }
            else if(events[i].events & EPOLLOUT)
            {
                if(!users[sockfd].write())
                {
                    users[sockfd].close_conn();
                }
            }
            else continue;
        }
    }

    close(epollfd);
    close(listenfd);
    delete[] users;
    delete pool;
    return 0;
}