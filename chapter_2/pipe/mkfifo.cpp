/*
 * @Description: FIFO Test
 * @Author: OuYang YIQI
 * @Date: 2023-07-27 23:22:39
 * @LastEditTime: 2023-07-28 00:04:24
 * @FilePath: /Linux/ProcessCommunication/mkfifo.cpp
 */

/*



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
    int ret = mkfifo("./fifo", 0664);
    if(ret == -1)
    {
        perror("mkfifo");
        exit(0);
    }
    return 0;
}