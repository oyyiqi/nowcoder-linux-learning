/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-03 20:43:47
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-03 20:57:52
 * @FilePath: /Linux/chapter_3/thread/producer_consumer.cpp
 * @Description: 生产者-消费者模型
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

#define PRODUCER_NUM 5
#define CONSUMER_NUM 5
#define BUF_SIZE 100
#include <vector>

vector<int> buf(100);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg)
{
    while (1)
    {
        usleep(1000);
        pthread_mutex_lock(&mutex);
        if (buf.size() < BUF_SIZE)
        {
            int num = rand() % 1000;
            buf.push_back(num);
            printf("(produce_%d): %d, size = %d\n", *(int *)arg, num, buf.size());
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
    return NULL;
}

void *consumer(void *arg)
{
    while (1)
    {
        usleep(2000);
        pthread_mutex_lock(&mutex);
        if (buf.size() > 0)
        {
            int back = buf.back();
            printf("(consum_%d): %d, size = %d\n", *(int *)arg, back, buf.size());
            buf.pop_back();
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
    return NULL;
}
int main()
{
    buf.resize(0);
    cout << "buf.size() = " << buf.size() << endl;
    cout << "buf.capacity() = " << buf.capacity() << endl;

    pthread_t tids_p[PRODUCER_NUM];
    pthread_t tids_c[CONSUMER_NUM];

    int args_p[PRODUCER_NUM], args_c[CONSUMER_NUM];
    for (int i = 0; i < PRODUCER_NUM; i++)
    {
        args_p[i] = i;
        pthread_create(&tids_p[i], NULL, producer, &args_p[i]);
        pthread_detach(tids_p[i]);
    }

    for (int i = 0; i < CONSUMER_NUM; i++)
    {
        args_c[i] = i;
        pthread_create(&tids_c[i], NULL, consumer, &args_c[i]);
        pthread_detach(tids_c[i]);
    }

    while(1)
    {
        sleep(10);
    }
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);

    return 0;
}