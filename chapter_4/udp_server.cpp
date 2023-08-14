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
using namespace std;

#define PORT 8080
#define BUF_SIZE 1024
int main()
{
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(socketfd, (sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    while (1)
    {
        char ip[16] = {0};
        unsigned short port;
        sockaddr_in client_addr;
        socklen_t ca_len = sizeof(client_addr);

        char buf[BUF_SIZE] = {0};
        int recv_len = recvfrom(socketfd, buf, BUF_SIZE, 0,
                                (sockaddr *)&client_addr, &ca_len);
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip));
        port = ntohs(client_addr.sin_port);

        if (recv_len == -1)
        {
            perror("recvfrom");
            goto error;
        }

        cout << "[RECEIVE] ---> " << ip << ":" << port << "  " << buf << endl;

        int send_len = sendto(socketfd, buf, strlen(buf) + 1, 0,
                              (sockaddr *)&client_addr, ca_len);
        if(send_len == -1)
        {
            perror("sendto");
            goto error;
        }

        cout << "[SEND] ---> " << ip << ":" << port << "  " << buf << endl;
    }

error:
{
    close(socketfd);
    exit(-1);
}

ipinfo:
{
    cout << "goto test" << endl;
}

    return 0;
}
