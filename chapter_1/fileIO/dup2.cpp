/*
 * @Description: dup2 test
 * @Author: OuYang YIQI
 * @Date: 2023-07-27 21:14:26
 * @LastEditTime: 2023-07-27 21:21:56
 * @FilePath: /Linux/IOTest/dup2.cpp
 */

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
using namespace std;

int main()
{

    int fd1 = open("./a.txt", O_RDWR);
    int fd2 = open("./b.txt", O_RDWR);
    cout << "fd1:" << fd1 << endl;
    cout << "fd2:" << fd2 << endl;
    dup2(fd1, fd2);
    cout << "fd1:" << fd1 << endl;
    cout << "fd2:" << fd2 << endl;
    return 0;
}
