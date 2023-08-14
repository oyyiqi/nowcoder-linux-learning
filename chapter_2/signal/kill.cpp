/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-30 16:02:14
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-30 16:31:33
 * @FilePath: /Linux/signaltest/kill.cpp
 * @Description: kill(pid_t pid, int sig)
 ***/

/*
    #include <sys/types.h>
    #include <signal.h>
    int kill (pid_t pid, int sig);
        功能 : 给某一进程或进程组发送信号
        参数 : 
            - pid : 
                case > 0 : 将信号发送给指定的进程
                case = 0 : 将信号发送给当前的进程组
                case = -1 : 将信号发送给每一个有权限发送的进程，除了Init进程
                case < -1 : 将信号发送给-pid进程组的进程
            - sig : 信号的编号,或者宏值，0表式不发送任何信号
                SIGKILL SIGINT SIGSTOP STGCONT ...

    #include <signal.h>
    int raise(int sig);
        1.功能 : 给当前进程发送信号
        2.参数 : 
            - sig : 要发送的信号
        3.返回值 :
            case 0 : 成功
            case 非0 : 失败

    #include <stdlib.h>
    void abort(void);
        1.功能 : 给当前进程发送 SIGABRT 信号，终止当前进程
        2.返回值 : 无

*/

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
    pid_t pid = fork();
    if(pid > 0)
    {
        // parent process
        sleep(2);
        cout << "parent send kill to child" << endl;
        kill(pid, SIGINT);
    }
    else if(pid == 0)
    {
        // child process
        for(int i = 0; i <= 5; i++)
        {
            cout << "I am child process" << endl;
            sleep(1);
        }
    }
    else
    {
        perror("fork");
        exit(0);
    }
    return 0;
}