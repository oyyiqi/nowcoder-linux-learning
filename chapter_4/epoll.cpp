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
using namespace std;

#define BACKLOG 5
#define MAX_CLIENT_NUM 1024
#define PORT 8080
#define BUF_SIZE 6
#define IPV4_LENGTH 16

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    bind(listen_fd, (sockaddr *)&addr, sizeof(addr));
    listen(listen_fd, BACKLOG);

    int epoll_fd = epoll_create(1);
    epoll_event epev;
    epev.data.fd = listen_fd;
    epev.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &epev);

    epoll_event epevs[MAX_CLIENT_NUM];
    memset(&epevs, '\0', sizeof(epevs));

    while (1)
    {
        int ret = epoll_wait(epoll_fd, epevs, MAX_CLIENT_NUM, -1);
        if (ret == -1)
        {
            perror("epoll_wait");
            exit(-1);
        }

        for (int i = 0; i < ret; i++)
        {
            if (epevs[i].data.fd == listen_fd)
            {
                sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);
                int client_fd = accept(listen_fd, (sockaddr *)&client_addr, &len);

                int flag = fcntl(client_fd, F_GETFL);
                // flag = flag | O_NONBLOCK;
                fcntl(client_fd, F_SETFL, flag);

                epev.events = EPOLLET;
                epev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &epev);

                char client_ip[IPV4_LENGTH];
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, IPV4_LENGTH);
                unsigned short client_port = ntohs(client_addr.sin_port);
                cout << "[CONNECT] ---> " << client_ip << ":" << client_port << endl;
            }

            else
            {
                while (1)
                {
                    if ((epevs[i].events & EPOLLRDHUP) || (epevs[i].events & EPOLLRDHUP))
                    {
                        if (epevs[i].events & EPOLLIN)
                        {
                            cout << "关闭连接,并发送EPOLLIN" << endl;
                        }
                    }
                    if (epevs[i].events & EPOLLIN)
                    {
                        char buf[BUF_SIZE] = {0};
                        int r_len = read(epevs[i].data.fd, buf, BUF_SIZE - 1);
                        if (r_len == -1)
                        {
                            if (errno == EAGAIN)
                            {
                                break;
                            }
                            perror("read");
                            close(epevs[i].data.fd);
                            return -1;
                        }

                        if (r_len == 0)
                        {
                            perror("read 0");
                            cout << "[DISCONNECT] ---> " << endl;
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, epevs[i].data.fd, NULL);
                            close(epevs[i].data.fd);
                            break;
                        }

                        else
                        {
                            cout << "[RECEIVE] ---> " << buf << endl;
                            write(epevs[i].data.fd, buf, strlen(buf) + 1);
                        }
                    }
                }
            }
        }
    }

    close(listen_fd);
    close(epoll_fd);
    return 0;
}