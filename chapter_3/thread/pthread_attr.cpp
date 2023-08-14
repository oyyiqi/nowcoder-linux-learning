/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-03 10:08:27
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-03 10:17:46
 * @FilePath: /Linux/chapter_3/thread/pthread_attr.cpp
 * @Description: 线程属性
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
    cout << "(child): " << pthread_self() << endl;
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    // create attr
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // set detach attr
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // create child thread
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);
    if (ret != 0)
    {
        char *errstr = strerror(ret);
        cout << "error: " << errstr << endl;
    }

    // get thread stack size
    size_t stack_size;
    ret = pthread_attr_getstacksize(&attr, &stack_size);
    if (ret == -1)
    {
        perror("pthread_attr_getstacksize");
        exit(0);
    }

    cout << "(main): stack_size = " << stack_size << endl;

    cout << "(main): " << pthread_self() << endl;

    pthread_attr_destroy(&attr);

    return 0;
}

/*
    TODO:上述示例中，有时候运行结果没有打印子线程的结果，有时候又打印了两次，这是为什么？
*/