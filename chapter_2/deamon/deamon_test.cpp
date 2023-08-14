/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-02 10:37:37
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-02 10:45:43
 * @FilePath: /Linux/Deamon/deamon_test.cpp
 * @Description: 测试创建守护进程时，不按规则会出现什么情况
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

/*** 
 * @description: 打印进程id,组id,以及会话id
 * @return {*}
 ***/
void print_id()
{
    cout << "pid:" << getpid();
    cout << " ppid:" << getppid();
    cout << " pgid:" << getpgid(getpid());
    cout << " sid:" << getsid(getpid()) << endl;
}

/*** 
 * @description: 信号处理函数
 * @param {int} sig:捕获到的信号
 * @return {*} void
 ***/
void catchSig(int sig)
{
    cout << "pid " << getpid() << " receive " << sig <<" signal ";
    cout << "do exit ...." << endl;
    exit(0);
}

/*** 
 * @description: 测试fork()不退出父进程的情况下，在子进程中调用setsid()
 * @return {*}
 * @test_result: 实际父进程退出时子进程没有退出，还是成为了守护进程
 ***/
void fork_no_exit()
{
    pid_t pid = fork();
    if (pid > 0)
    {
        cout << "parent -------------------------------" << endl;
        print_id();

        struct sigaction act;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        act.sa_handler = catchSig; 
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGHUP, &act, NULL);
        
        while (1)
        {
            sleep(10);
        }
    }

    else if (pid == 0)
    {
        cout << "child before setsid ....." << endl;
        print_id();
        int sid = setsid();
        if (sid == -1)
        {
            perror("setsid");
            exit(0);
        }
        cout << "child after setsid ....." << endl;
        print_id();

        struct sigaction act;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        act.sa_handler = catchSig; 
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGHUP, &act, NULL);
        while(1)
        {
            sleep(10);
        }
    }
}

void no_fork()
{
/*** 
 * @description:测试不fork()直接调用setsid会发生什么 
 * @return {*}
 ***/    
    int ret = setsid();
    if(ret == -1)
    {
        perror("setsid");
    }
    exit(0);
}

int main()
    {
    // fork_no_exit();
    no_fork();
    return 0;
}