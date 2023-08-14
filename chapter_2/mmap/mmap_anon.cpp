/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-30 14:41:52
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-30 15:03:20
 * @FilePath: /Linux/mmapTest/mmap_anon.cpp
 * @Description:
 ***/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

#define ANONYMOUS_SIZE 4096
int main()
{
    auto ptr = mmap(NULL, ANONYMOUS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(0);
    }

    pid_t pid = fork();

    if (pid > 0)
    {
        // parent process
        strcpy((char *)ptr, "hello world");
        wait(NULL);
    }

    if(pid == 0)
    {
        // child process
        sleep(1);
        cout << (char *)ptr << endl;
    }

    munmap(ptr, ANONYMOUS_SIZE);

    return 0;
}
