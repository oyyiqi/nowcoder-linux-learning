/*
 * @Description: fifo read
 * @Author: OuYang YIQI
 * @Date: 2023-07-27 23:27:03
 * @LastEditTime: 2023-07-27 23:45:45
 * @FilePath: /Linux/ProcessCommunication/read_fifo.cpp
 */

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
using namespace std;

int main()
{
    int fd = open("./fifo", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        exit(0);
    }

    while (1)
    {
        char buf[1024];
        int len = read(fd, buf, sizeof(buf));
        if (len == 0)
        {
            cout << "write server shut down" << endl;
            break;
        }
        cout << "receive:" << buf << endl;
    }

    close(fd);

    return 0;
}
