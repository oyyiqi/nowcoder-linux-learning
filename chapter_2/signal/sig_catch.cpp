/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-30 19:06:22
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-31 12:20:37
 * @FilePath: /Linux/signaltest/sig_catch.cpp
 * @Description: signal catch
 ***/

/*
    #include <signal.h>
    typedef void (*sighandler_t)(int);
    sighandler_t signal(int signum, sighandler_t handler);
        功能 : 捕捉信号，执行自定义的信号处理函数（SIG_KILL,SIG_STOP无法被捕捉）
        参数 :
            - signum : 要捕捉的信号（尽量使用宏，因为信号的值在不同架构的系统上可能有所不同）
            - handler ： 决定了如何处理捕捉的信号
                case SIG_IGN : 忽略信号
                case SIG_DFL : 使用信号默认的行为
                case 回调函数 : 这个函数内核调用，程序员只负责写【存疑】
        返回值 :
        
            成功：返回上一次注册的信号处理函数的地址，第一次调用返回NULL
            失败：返回SIG_ERR，并设置errono

    int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
        功能 : 检查或改变进程对信号的行为
        参数 :
            - signum : 需要捕捉的信号
            - act : 捕捉到信号后的处理动作
            - oldact : 传出参数，传出上次进程对该信号的处理，一般不使用，设为NULL;

            struct sigaction {
                void (*sa_handler)(int);
                    信号处理函数指针
                void (*sa_sigaction)(int, siginfo_t *, void *);
                    不常用。通常和sa_handler二选一，设置其中一个时，另一个设置成NULL
                sigset_t sa_mask;
                    临时阻塞信号集，在信号捕捉函数执行过程中，临时阻塞某些信号
                int sa_flags;
                    0:表示使用sa_handler,SA_SIGINFO:表示使用sa_sigaction
                void (*sa_restorer)(void);
                    已废弃，设置成NULL即可
           };
        返回值 :
            case 0 : 成功
            case -1 : 失败
    【由于一些标准问题，建议使用sigaction，更加通用~】
*/
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

static int num = 1;

void signalCatch(int signal)
{
    cout << "catching a signal:" << signal << endl;
    cout << "this " << num++ << " times catch" << endl;
}

/***
 * @description: test signal()
 * @return void
 ***/
void signalTest()
{

    auto ret = signal(SIGALRM, signalCatch);

    itimerval new_value;

    // start timer after 3s
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &new_value, NULL);
    cout << "set timer, wait trigger ..." << endl;
    int time = 0;
    while (1)
    {
        cout << "time : " << ++time << endl;
        sleep(1);
    }
}

/*** 
 * @description: Test sigaction()
 * @return void
 ***/
void sigactionTest()
{

    struct sigaction act; //定义捕捉到函数后如何处理的结构体
    act.sa_flags = 0;     // 使用sa_handler处理
    sigemptyset(&act.sa_mask);  //初始化 sa_mask
    act.sa_handler = signalCatch; //绑定信号处理函数

    auto ret = sigaction(SIGALRM, &act, NULL); //捕捉SIGALRM函数

    struct itimerval new_value;

    // start timer after 3s
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &new_value, NULL);
    cout << "set timer, wait trigger ..." << endl;
    int time = 0;
    while (1)
    {
        cout << "time : " << ++time << endl;
        sleep(1);
    }
}
int main()
{
    sigactionTest();
    return 0;
}