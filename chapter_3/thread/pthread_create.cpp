/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-02 19:06:02
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-02 19:14:25
 * @FilePath: /Linux/chapter_3/thread/pthread_create.cpp
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
    int thread_arg = *((int *)arg);
    for(int i = 0; i < 5; i++)
    {
        sleep(1);
        cout << "i am new thread, thread_arg = " << thread_arg << endl;
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    int arg = 1;
    int ret = pthread_create(&tid, NULL, callback, &arg);

    cout << "wait new thread over..." << endl;
    pthread_join(tid, NULL);
    cout << "thread : " << tid << " over" << endl;

    return 0;
}