/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-27 21:26:59
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-02 17:37:48
 * @FilePath: /Linux/pipe/psaux.cpp
 * @Description: 实现 ps aux | grep xxx
 ***/


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
    if(pid > 0)
    {
        // parent proccess
        close(fd[1]);
        char buf[1024];
        while((read(fd[0], buf, sizeof(buf))))
        {
            cout << buf;
        }
    }
    else if(pid == 0)
    {
        // child process
        close(fd[0]);
        int dupret = dup2(fd[1], STDOUT_FILENO);
        if(dupret == -1)
        {
            perror("dup2");
            exit(0);
        }
        
        int pipelen = fpathconf(fd[1], _PC_PIPE_BUF);  // get pipe size
        execlp("ps", "ps", "aux", NULL);
    }
    else
    {
        perror("fork");
        exit(0);
    }
    
    return 0;
}