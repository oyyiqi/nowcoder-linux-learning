/*
 * @Description: set pipe no block
 * @Author: OuYang YIQI
 * @Date: 2023-07-27 23:01:43
 * @LastEditTime: 2023-07-27 23:10:32
 * @FilePath: /Linux/ProcessTest/pipeNoBlock.cpp
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
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe");
        exit(0);
    }
    pid_t pid = fork();
    // set no block
    auto flag = fcntl(fd[0], F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd[0], F_SETFL, flag);

    if(pid == 0)
    {
        // child process
        close(fd[0]);
        // do write;
    }
    else if (pid > 0)
    {
        // parent process
        close(fd[1]);  // close write
        // do read
    }
    else
    {
        perror("fork");
        exit(0);
    }
    
    return 0;
}
