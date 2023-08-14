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
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    sockaddr_un c_addr;
    socklen_t c_len = sizeof(c_addr);

    sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server.sock");
    int ret = bind(lfd, (sockaddr *)&addr, sizeof(addr));

    if (ret == -1)
    {
        perror("bind");
    }

    ret = listen(lfd, 5);
    if (ret == -1)
    {
        perror("listen");
    }

    int cfd = accept(lfd, (sockaddr *)&c_addr, &c_len);

    if (cfd == -1)
    {
        perror("accept");
        goto error;
    }

    cout << "client socket file path = " << c_addr.sun_path << endl;

    while(1)
    {
        char buf[1024];
        int len = recv(cfd,  buf, sizeof(buf), 0);

        if(len == -1)
        {
            perror("recv");
            goto error;
        }
        else  if(len == 0)
        {
            cout << "client closed ..." << endl;
        }

        else
        {
            cout << "receive : " << buf << endl;
        }

        send(cf`d, buf, strlen(buf) + 1, 0);
        
        cout << "send : " << buf << endl;
    }

error:
{
    close(lfd);
    exit(-1);
}

    return 0;
}