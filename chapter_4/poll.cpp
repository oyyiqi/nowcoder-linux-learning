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
#include <sys/select.h>
#include <poll.h>
using namespace std;

#define BACKLOG 5
#define MAX_CLIENT_NUM 1024
#define PORT 8080
#define BUF_SIZE 1024
#define IPV4_LENGTH 16

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    bind(listenfd, (sockaddr *)&addr, sizeof(addr));
    listen(listenfd, BACKLOG);

    pollfd fds[MAX_CLIENT_NUM];

    for(int i = 0; i < MAX_CLIENT_NUM; i++)
    {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }

    fds[0].fd = listenfd;

    int cur_max_fd = 1;
    while(1)
    {
        int ret = poll(fds, cur_max_fd, -1);
        if(ret == -1)
        {
            perror("poll");
            exit(-1);
        }

        if(fds[0].revents & POLLIN)
        {
            sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr);
            int client_fd = accept(listenfd, (sockaddr *)&client_addr, &len);
            char client_ip[IPV4_LENGTH];
            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, IPV4_LENGTH);
            unsigned short client_port  = ntohs(client_addr.sin_port);
            for(int i = 0; i < MAX_CLIENT_NUM; i++)
            {
                if(fds[i].fd == -1)
                {
                    fds[i].fd = client_fd;
                    fds[i].events = POLLIN;
                    break;
                }
            }

            cout << "[CONNECT] ---> " << client_ip << ":" << client_port << endl;
            cur_max_fd = max(cur_max_fd, client_fd);
        }

        for(int i =  1; i <= cur_max_fd; i++)
        {
            if(fds[i].revents & POLLIN)
            {
                char buf[BUF_SIZE] = {0};
                int r_len = read(fds[i].fd, buf, BUF_SIZE);
                if(r_len == -1)
                {
                    perror("read");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    exit(-1);
                }
                if(r_len == 0)
                {
                    cout << "[DISCONNECT] ---> " << endl;
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    continue;
                }
                cout << "[RECEIVE] ---> " << buf << endl;

                int w_len = write(fds[i].fd, buf, strlen(buf) + 1);
            }
        }
    }

    for(int i = 0; i < MAX_CLIENT_NUM; i++)
    {
        if(fds[i].fd != -1)
        {
            close(fds[i].fd);
            fds[i].fd = -1;
        }
    }
    
    close(listenfd);
    return 0;
}

