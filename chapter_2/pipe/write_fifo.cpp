/*
 * @Description: fifo write
 * @Author: OuYang YIQI
 * @Date: 2023-07-27 23:27:03
 * @LastEditTime: 2023-07-27 23:49:39
 * @FilePath: /Linux/ProcessCommunication/write_fifo.cpp
 */


#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

int main()
{
    int ret = access("./fifo", F_OK);
    if(ret == -1)
    {
        ret = mkfifo("./fifo", 0664);
        if(ret == -1)
        {
            perror("mkfifo");
            exit(0);
        }
        
    }

    int fd = open("./fifo", O_WRONLY); 
    if(fd == -1)
    {
        perror("open");
        exit(0);
    }
    
    for(int i = 0; i < 100; i++)
    {
        char buf[1024];
        sprintf(buf, "hello, %d", i);
        write(fd, buf, strlen(buf));
        cout << "write data : " << buf << endl;
        sleep(1);
    }

    close(fd);
    return 0;
}


