/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-03 08:59:07
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-03 09:59:51
 * @FilePath: /Linux/chapter_3/thread/pthread_cancel.cpp
 * @Description: 
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
#include <pthread.h>
using namespace std;


void *callback(void *arg)
{
    // sleep(1);   // sleep也是一个取消点
    for(int i = 0; i < 100; i++)
    {
        cout << "(child): " << i << flush <<" ";
        // cout << endl; 会刷新缓冲区，是一个取消点
    }
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, callback, NULL);
    
    // cancel thread
    pthread_cancel(tid);

    // main thread do something
    sleep(1);
    for(int i = 0; i < 10; i++)
    {
        cout << "(main): " << i << endl;
        // sleep(1);
    }
    return 0;
}

/*
    输出有问题，主线程sleep(1）后，后面的for循环没执行就退出了
*/