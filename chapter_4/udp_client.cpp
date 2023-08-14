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

#define SERVER_IP "7.249.5.156"
#define SERVER_PORT 8080
#define BUF_SIZE 1024

int main()
{
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    while (1)
    {
        char buf[BUF_SIZE] = {0};
        char ip[16] = {0};
        unsigned short port = 0;

        sockaddr_in server_addr;
        socklen_t ca_len = sizeof(server_addr);
        inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr.s_addr);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);

        string msg = "";
        getline(cin, msg);

        
        int send_len = sendto(socketfd, msg.c_str(), msg.length() + 1, 0,
                              (sockaddr *)&server_addr, ca_len);
        if (send_len == -1)
        {
            perror("sendto");
            goto error;
        }

        cout << "[SEND] ---> " << SERVER_IP << ":" << SERVER_PORT << "  " << msg << endl;


        int recv_len = recvfrom(socketfd, buf, BUF_SIZE, 0,
                                NULL, NULL);
        inet_ntop(AF_INET, &server_addr.sin_addr.s_addr, ip, sizeof(ip));
        port = ntohs(server_addr.sin_port);

        if (recv_len == -1)
        {
            perror("recvfrom");
            goto error;
        }

        cout << "[RECEIVE] ---> " << SERVER_IP << ":" << SERVER_PORT << "  " << buf << endl;
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
