/*
 * @Description: Chat A process
 * @Author: OuYang YIQI
 * @Date: 2023-07-28 09:27:06
 * @LastEditTime: 2023-07-28 10:29:51
 * @FilePath: /Linux/ProcessCommunication/chatA.cpp
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
    // 1. if fifo not exit, create it
    int ret = -1;
    ret = access("./fifo1", F_OK);
    if (ret == -1)
    {
        ret = mkfifo("./fifo1", 0664);
        if (ret == -1)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("./fifo2", F_OK);
    if (ret == -1)
    {
        ret = mkfifo("./fifo2", 0664);
        if (ret == -1)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    // 2. open fifo1 by read only, open fifo2 by write only
    int fd_read = open("./fifo1", O_RDONLY);
    if (fd_read == -1)
    {
        perror("open fifo1");
        exit(0);
    }

    int fd_write = open("./fifo2", O_WRONLY);
    if (fd_write == -1)
    {
        perror("open fifo2");
        exit(0);
    }
    cout << "hello" << endl;
    // 3.read & write data
    while (1)
    {
        char buf[128];
        memset(buf, 0, sizeof(buf));
        string str;
        cout << "send : ";
        getline(cin, str);
        int i;
        for (i = 0; i < str.length(); i++)
        {
            buf[i] = str[i];
        }
        str[i] = '\0';

        int len_write = write(fd_write, buf, strlen(buf));
        if (len_write == -1)
        {
            perror("write");
        }

        // cout << "wait receive data ... " << endl;
        memset(buf, 0, sizeof(buf));
        int len_read = read(fd_read, buf, sizeof(buf));
        if (len_read == -1)
        {
            perror("read");
        }
        cout << "receive : " << buf << endl;
    }

    return 0;
}
