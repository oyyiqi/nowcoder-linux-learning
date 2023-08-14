/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-30 16:32:41
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-30 17:04:11
 * @FilePath: /Linux/signaltest/alarm.cpp
 * @Description: 定时器
 ***/

/*
    #include <unistd.h>
    unsigned int alarm(unsigned int seconds);
        功能 : 设置一个定时器，当计时为0时，给当前进程发送SIGALARM信号
              SIGALARM : 该信号默认终止当前的进程
        参数 :
            - unsigned int seconds : 倒计时的时间(s)，如果为0则定时器无效
        返回值 :
            case 之前没有定时器 : 0
            case 之前有定时器 : 返回之前的定时器剩余的时间
    【注:同一时间一个程序只能有一个定时器，前面的定时器将会无效并返回剩余的时间】
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

/*** 
 * @description: 定时器使用规则
 * @return {*}
 ***/
void test_alarm()
{
    int ret = alarm(5);
    cout << "first alarm ret:" << ret << endl;
    sleep(1);
    ret = alarm(10);
    cout << "sencond alarm ret:" << ret << endl;
    while (1)
    {
        cout << "I am sleeping" << endl;
        sleep(1);
    }
}

/***
 * @description: 测试1s中内计算机能数多少个数
 * @return {*}
 ***/
void test_one_sencond()
{
    alarm(1);
    int i = 0;
    while (1)
    {
        printf("%d\n", i++);
    }
}

int main()
{
    test_one_sencond();
    return 0;
}
