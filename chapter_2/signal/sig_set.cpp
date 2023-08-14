/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-30 22:57:24
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-31 10:51:57
 * @FilePath: /Linux/signaltest/sig_set.cpp
 * @Description: sig set
 ***/

/*
    int sigemptyset(sigset_t *set);
        信号集全置为0
    int sigfullset(sigset_t *set);
        信号集全置为1
    int sigaddset(sigset_t *set, int signum);
        置signum标志位为1
    int sigdelset(sigset_t *set, int signum);
        置signum标志位为0
    int sigismember(const sigset_t *set, int signum);
        判断signum标志位是不是1
    int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
        功能 : 将自定义信号集推送到内核中
        参数 :
            - how : 如何对内核阻塞信号集进行处理
                case SIG_BLOCK : 将自定义的阻塞信号集合并到到内核阻塞信号集中
                    假设内核默认阻塞信号集是mask，则mask = mask | set
                case SIG_UNBLOCK : 从内核阻塞信号集中取消某些信号的阻塞状态，即mask = mask & ~set
                case SIG_SETMASK : 用自定义的信号集覆盖内核阻塞信号集
            - set : 自定义的信号集。可以为NULL，此时how参数将被忽略，并在old_set中返回当前的mask
            - old_set : 传出参数，保存更改前内核的阻塞信号集，可以设置为NULL,表示不保存
        返回值 :
            case 0 : 成功
            case -1 : 失败，并设置错误号：EFAULT、EINVAL
    int sigpending(sigset_t *set);
        获取内核中的未决信号集,从传出参数set中返回
*/

// 编写一个程序，把所有的常规信号(1-31)处于未决状态的信号打印到屏幕

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
#include <signal.h>
using namespace std;

int main()
{
    // creat and init a sigset
    sigset_t mySet;
    sigemptyset(&mySet);

    // add sig 2 and sig 3 into mySet
    sigaddset(&mySet, SIGINT);
    sigaddset(&mySet, SIGQUIT);

    // modify kernel sig set by mySet
    int ret = sigprocmask(SIG_BLOCK, &mySet, NULL);
    if (ret == -1)
    {
        perror("sigprocmask");
        exit(0);
    }

    // get and print pending set
    int time = 0;
    while (1)
    {
        // get pending set
        sigset_t sigsetPending;
        sigemptyset(&sigsetPending);
        if (sigpending(&sigsetPending) == -1)
        {
            perror("sigpending");
            exit(0);
        }

        int num = 1024 / (8 * sizeof(unsigned long int));
        for (int i = 1; i < num; i++)
        {
            if (sigismember(&sigsetPending, i) == 1)
            {
                cout << "1";
            }
            else if (sigismember(&sigsetPending, i) == 0)
            {
                cout << "0";
            }
            else
            {
                perror("sigismember");
                exit(0);
            }
        }
        cout << endl;
        if (time == 10)
        {
            cout << "unblock" << endl;
            sigprocmask(SIG_UNBLOCK, &mySet, NULL);
        }
        sleep(1);
        time++;
    }
    return 0;
}