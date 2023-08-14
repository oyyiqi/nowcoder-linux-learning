#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
using namespace std;

#define BACKLOG 5
#define MAX_CLIENT_NUM 3

struct ClientInfo
{
    int c_sockfd;            // socket文件描述符
    pthread_t tid;           // 线程号
    struct sockaddr_in addr; // 客户端ip,port等信息
};

ClientInfo clientInfomations[MAX_CLIENT_NUM];

void initSigAction(struct sigaction &act, void (*handler)(int sig))
{
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;
}

void stringToCharArray(string str, char buf[])
{
    int i = 0;
    for (; i < str.length(); i++)
    {
        buf[i] = str[i];
    }
    buf[i] = '\0';
}

void sigChildCatch(int signal)
{
    while (1)
    {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret > 0)
        {
            printf("cilent %d disconnected\n", ret);
        }
        if (ret == 0)
        {
            sleep(2);
            continue;
        }
        if (ret == -1)
        {
            cout << "all client disconnected ~" << endl;
            break;
        }
    }
}

void *do_thread(void *arg)
{
    ClientInfo *p_ClientInfo = (ClientInfo *)arg;
    char client_IP[16] = {0};
    inet_ntop(AF_INET, &p_ClientInfo->addr.sin_addr, client_IP, sizeof(client_IP));
    unsigned short port = ntohs(p_ClientInfo->addr.sin_port);
    cout << "[" << client_IP << ":" << port << "] " << "connected" << endl;
    while (1)
    {
        char recv_buf[1024] = {0};
        int len = read(p_ClientInfo->c_sockfd, recv_buf, sizeof(recv_buf));
        if (len > 0)
        {
            cout << "[" << client_IP << ":" << port << "] " << recv_buf << endl;
        }

        if (len == 0)
        {
            cout << "[" << client_IP << ":" << port << "] "
                 << "disconnect" << endl;
            break;
        }

        if (len == -1)
        {
            perror("read");
            close(p_ClientInfo->c_sockfd);
            exit(-1);
        }

        char send_buf[2048] = {0};
        sprintf(send_buf, "server received %s", recv_buf);

        len = write(p_ClientInfo->c_sockfd, send_buf, strlen(send_buf) + 1);
        if (len == -1)
        {
            perror("write");
            close(p_ClientInfo->c_sockfd);
            exit(-1);
        }
    }
    close(p_ClientInfo->c_sockfd);
    p_ClientInfo->c_sockfd = 0;
    cout << "success free c_sockfd" << endl;
    pthread_exit(NULL);
}
int main()
{

    memset(&clientInfomations, 0, sizeof(clientInfomations));
    
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(listenfd, (sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    ret = listen(listenfd, BACKLOG);

    while (1)
    {
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int c_sockfd = accept(listenfd, (sockaddr *)&client_addr, &len);
        if (c_sockfd == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("accept");
            exit(-1);
        }

        ClientInfo *p_ClientInfo = NULL;
        while (1)
        {
            for (int i = 0; i < MAX_CLIENT_NUM; i++)
            {
                if (clientInfomations[i].c_sockfd == 0)
                {
                    p_ClientInfo = &clientInfomations[i];
                    break;
                }
            }
            if (!p_ClientInfo)
            {
                sleep(1);
                cout << "current connect already up to max ..." << endl;
                continue;
            }
            break;
        }

        p_ClientInfo->addr = client_addr;
        p_ClientInfo->c_sockfd = c_sockfd;

        pthread_create(&p_ClientInfo->tid, NULL, do_thread, p_ClientInfo);
        pthread_detach(p_ClientInfo->tid);
    }

    close(listenfd);

    return 0;
}

void server()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        exit(-1);
    }
    sockaddr_in skaddr;
    skaddr.sin_family = AF_INET;
    skaddr.sin_addr.s_addr = INADDR_ANY;
    skaddr.sin_port = htons(8031);
    // skaddr.sin_zero 内存对齐？

    int ret = bind(sockfd, (sockaddr *)&skaddr, sizeof(skaddr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    ret = listen(sockfd, BACKLOG);
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }
    cout << "listen over" << endl;

    sockaddr_in client_addr;
    socklen_t socklen = sizeof(client_addr);
    int c_sockfd = accept(sockfd, (sockaddr *)&client_addr, &socklen);
    if (c_sockfd == -1)
    {
        perror("accept");
        exit(-1);
    }
    cout << "accept over" << endl;

    char clientIP[16];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(client_addr.sin_port);
    cout << "server:"
         << "connect with " << clientIP << ":" << clientPort << " success!" << endl;

    /******************read & write **********************/
    char recv_buf[1024] = {0};
    int len = read(c_sockfd, recv_buf, sizeof(recv_buf));
    if (len == -1)
    {
        perror("read");
        exit(-1);
    }
    else if (len == 0)
    {
        cout << "client closed" << endl;
    }
    else
    {
        cout << "server receive :  " << recv_buf << endl;
    }

    char msg[] = "hello, i am server";
    len = write(c_sockfd, msg, size_t(sizeof(msg)));
}

void server_multiprocess()
{

    struct sigaction act;
    initSigAction(act, sigChildCatch);
    sigaction(SIGCHLD, &act, NULL);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    // inet_pton(AF_INET, "196.168.1.103", &addr.sin_addr.s_addr);
    int ret = bind(sockfd, (sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }
    ret = listen(sockfd, BACKLOG);
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }

    while (1)
    {
        sockaddr_in c_addr;
        socklen_t len = sizeof(c_addr);
        int c_sockfd = accept(sockfd, (sockaddr *)&c_addr, &len);
        if (c_sockfd == -1)
        {
            if (errno == EINTR)
                continue;
            perror("accept");
            exit(-1);
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            char client_ip[16];
            inet_ntop(AF_INET, &c_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
            cout << "【" << getpid() << "】: connect with "
                 << client_ip << ":" << ntohs(c_addr.sin_port) << endl;
            char recv_buf[1024] = {0};
            while (1)
            {
                int len = read(c_sockfd, recv_buf, sizeof(recv_buf));
                if (len == -1)
                {
                    perror("read");
                    exit(-1);
                }
                if (len > 0)
                {
                    cout << "【" << getpid() << "】: receive " << recv_buf << endl;
                }
                else if (len == 0)
                {
                    cout << client_ip << ":" << ntohs(c_addr.sin_port) << " disconnected" << endl;
                    break;
                }
                char send_buf[2048];
                sprintf(send_buf, "received %s", recv_buf);
                int write_len = write(c_sockfd, send_buf, strlen(send_buf) + 1);
                if (write_len == -1)
                {
                    perror("write");
                    exit(-1);
                }
                memset(recv_buf, 0, sizeof(recv_buf));
            }
            close(c_sockfd);
            cout << "close fd" << endl;
            exit(0);
        }

        else if (pid > 0)
        {
            continue;
        }

        else
        {
            perror("fork");
        }
    }
}