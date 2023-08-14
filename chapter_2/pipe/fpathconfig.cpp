/*
 * @Description: fpathconfig test
 * @Author: OuYang YIQI
 * @Date: 2023-07-27 20:36:20
 * @LastEditTime: 2023-07-27 20:39:23
 * @FilePath: /Linux/ProcessCommunication/fpathconfig.cpp
 */

#include<iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main()
{
    int fd[2];
    int ret = pipe(fd);
    long size = fpathconf(fd[0], _PC_PIPE_BUF);
    cout << "pipe size is " << size << endl;
    return 0;
}
