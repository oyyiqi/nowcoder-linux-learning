/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-30 18:09:12
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-30 22:19:10
 * @FilePath: /Linux/signaltest/setitimer.cpp
 * @Description: setitimer
 ***/

/*
    #include <sys/time.h>
    int getitimer(int which, struct itimerval *curr_value);
    int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
        功能 : 设置定时器
        参数 : 
            - int which : 定时器以什么时间计时
                case ITIMER_REAL : 真实物理时间，时间到达发送SIGALRM信号（常用）
                case ITIMER_VIRTUAL : 用户态时间，不包括内核态等其他时间消耗，时间达到发送SIGVTALRM信号
                case ITIMER_PROF : 包含用户态和内核态时间，不包括其他时间消耗（如硬件中断）
            - const struct itimerval *new_value :

                struct itimerval {
                    struct timeval it_interval; // 定时器开始后每过多久触发一次
                    struct timeval it_value;    // 多久后开始定时器
                };

                struct timeval {
                    time_t      tv_sec;         // seconds 秒
                    suseconds_t tv_usec;        // microseconds 微秒 
                };
            - struct itimerval *old_value ： 上一次的定时参数，一般不用，设置成NULL
        返回值 : 
            case 0 : 成功
            case -1 : 失败
*/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

int main()
{
    struct itimerval new_value;

    // 多久后开始触发
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;

    // 触发后，每过多久再次触发
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
    if(ret == -1)
    {
        perror("setitimer");
        exit(0);
    }

    while(1)
    {
        cout << "I am sleep" << endl;
        sleep(1);
    }   
    

    return 0;
}