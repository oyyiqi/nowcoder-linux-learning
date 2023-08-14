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
using namespace std;

#define BACKLOG 5

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    bind(listenfd, (sockaddr *)&addr, sizeof(addr));
    listen(listenfd, BACKLOG);

    fd_set u_read_set,k_read_set;
    FD_ZERO(&u_read_set);
    FD_SET(listenfd, &u_read_set);

    int MAXFD = listenfd;

    while(1)
    {
        k_read_set = u_read_set;
        int ret = select(MAXFD + 1, &k_read_set, NULL, NULL, NULL);
        if(ret == -1)
        {
            perror("select");
            exit(-1);
        }

        if(FD_ISSET(listenfd, &k_read_set))
        {
            sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr);
 
            int clientfd = accept(listenfd, (sockaddr *)&client_addr, &len);


            FD_SET(clientfd, &u_read_set);

            MAXFD = max(MAXFD, clientfd);
        }

        for(int i = listenfd + 1; i < MAXFD + 1; i++)
        {
            if(FD_ISSET(i, &k_read_set))
            {
                char buf[1024] = {0};
                int r_len = read(i, buf, sizeof(buf));
                if(r_len == -1)
                {
                    perror("read");
                    exit(-1);
                }

                else if (r_len == 0)
                {
                    cout << "client closed" << endl;
                    FD_CLR(i, &u_read_set);
                    close(i);
                }

                else
                {
                    cout << "receive data : "  << buf << endl;
                    write(i, buf, strlen(buf) + 1);
                }
            }
        }

    }
    
    close(listenfd);
    return 0;
}

