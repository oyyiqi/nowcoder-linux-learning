/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-30 15:51:37
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-30 15:55:55
 * @FilePath: /Linux/signaltest/core.cpp
 * @Description: test core
 ***/

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
using namespace std;

int main()
{
    char * buf;
    strcpy(buf, "hello");
    return 0;
}