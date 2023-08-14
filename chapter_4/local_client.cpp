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
#include <sys/un.h>
using namespace std;

int main()
{
    int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // sockaddr_un addr;
    // addr.sun_family = AF_LOCAL;
    // strcpy(addr.sun_path, "client.sock");


    sockaddr_un s_addr;
    s_addr.sun_family = AF_LOCAL;
    strcpy(s_addr.sun_path, "server.sock");
    int ret = connect(sockfd, (sockaddr *)&s_addr, sizeof(s_addr));
    if(ret == -1)
    {
        perror("connect");
        close(sockfd);
        exit(-1);
    }


    while(1)
    {
        string msg;
        getline(cin, msg);
        int w_len = send(sockfd, msg.c_str(), msg.length() + 1, 0);
        if(w_len == -1)
        {
            perror("send");
            close(sockfd);
            exit(-1);
        }

        cout << "send : " << msg << endl;

        char buf[1024] = {0};
        int r_len = recv(sockfd, buf, sizeof(buf) + 1, 0);
        if(r_len == 0)
        {
            cout << "server closed ..." << endl;
            continue;
        }

        if(r_len == -1)
        {
            perror("recv");
            close(sockfd);
            exit(0);
        }

        cout << "receive : " << buf << endl;
        
    }

    close(sockfd);

    return 0;
}