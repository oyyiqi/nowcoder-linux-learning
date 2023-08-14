/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-31 19:17:31
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-31 19:35:09
 * @FilePath: /Linux/signaltest/sig_child.cpp
 * @Description: SIGCHILD
 ***/
/*
    SIGCHLD信号产生的条件
        1.子进程终止时
        2.子进程接收到SIGSTOP信号停止时
        3.子进程处在停止态，接受到SIGCONT后唤醒时
    以上三种情况子进程都会给父进程发送SIGCHLD信号，父进程默认忽略该信号
*/

/*使用SIGCHLD解决僵尸进程问题*/

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

void catchSigChild(int signal)
{
    cout << "catched signal : " << signal << endl;
    while (1)
    {
        sleep(1);
        pid_t pid = waitpid(-1, NULL, WNOHANG);
        if (pid > 0)
        {
            cout << "success recycle process" << pid << endl;
        }
        else
        {
            break;
        }
    }
}

int main()
{
    pid_t pid;

    struct sigaction act;
    act.sa_flags = 0;               // 表式使用单参数处理函数
    sigemptyset(&act.sa_mask);      // 初始化信号集
    act.sa_handler = catchSigChild; // 绑定信号处理函数
    sigaction(SIGCHLD, &act, NULL);

    for (int i = 0; i < 10; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            break;
        }
    }

    if (pid > 0)
    {
        while (1)
        {
            cout << "I am parent, I am sleeping" << endl;
            sleep(1);
        }
    }

    else if (pid == 0)
    {
        cout << "I am child : " << getpid() << endl;
    }

    else
    {
        perror("fork");
    }

    return 0;
}