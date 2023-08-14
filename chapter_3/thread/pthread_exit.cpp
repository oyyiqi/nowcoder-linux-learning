/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-02 19:35:39
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-02 19:46:33
 * @FilePath: /Linux/chapter_3/thread/pthread_exit.cpp
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


/*
    pthread_equal(pthread_t t1, pthread_t t2);
        功能:比较两个线程ID是否相等
*/


void *callback(void *arg)
{
    int thread_arg = *((int *)arg);
    for(int i = 0; i < 5; i++)
    {
        sleep(1);
        cout << "(child thread) : thread_arg = " << thread_arg << endl;
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    int arg = 1;
    int ret = pthread_create(&tid, NULL, callback, &arg);

    cout << "(main thread): tid = " << pthread_self() << endl;

    pthread_exit(NULL);  // 
    
    return 0;
}