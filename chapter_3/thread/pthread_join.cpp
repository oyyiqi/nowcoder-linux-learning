/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-02 19:47:43
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-02 21:35:34
 * @FilePath: /Linux/chapter_3/thread/pthread_join.cpp
 * @Description: 线程回收，与线程状态返回
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
    cout << "(child) : receive "<< *(int *)arg << endl;
    int* ret = new int(1);
    // pthread_exit(ret);
    return ret;
}

int main()
{
    pthread_t tid;
    int arg = 10;
    int ret = pthread_create(&tid, NULL, callback, &arg);
    // cout << "(main) : join ...." << endl;
    // int * msg;
    // pthread_join(tid, (void **)&msg);
    sleep(1);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    

    int threadCount = 0;
    size_t stackSize;
    pthread_attr_getstacksize(&attr, &stackSize);
    threadCount = stackSize / PTHREAD_STACK_MIN;

    
    cout << "current thread num = " << threadCount << endl;
    // cout << "(main) : receive child message " << *msg << endl;
    // free(msg);

    return 0;
}