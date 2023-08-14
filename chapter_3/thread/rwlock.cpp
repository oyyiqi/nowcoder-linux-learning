/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-03 16:06:18
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-03 18:53:00
 * @FilePath: /Linux/chapter_3/thread/rwlock.cpp
 * @Description: 读写锁
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

#define READ_NUM 5
#define WRITE_NUM 4

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int rw_num = 0;

int rdlock_num = 0;

void *read_num(void *arg)
{
    while (1)
    {
        if (pthread_rwlock_tryrdlock(&rwlock) == 0)
        {
            // do read
            pthread_mutex_lock(&mutex);
            rdlock_num++;
            string str = "";
            for(int i = 0; i < rdlock_num; i++)
            {
                str += "*";
            }
            str += '\0';
            cout << "(read_" << *(int *)arg << "):"<< rdlock_num << endl;
            pthread_mutex_unlock(&mutex);


            // usleep(1000);


            pthread_mutex_lock(&mutex);
            rdlock_num--;
            str = "";
            for(int i = 0; i < rdlock_num; i++)
            {
                str += "*";
            }
            str += '\0';
            cout << "(read_" << *(int *)arg << "):"<< rdlock_num << endl;
            pthread_mutex_unlock(&mutex);

            pthread_rwlock_unlock(&rwlock);
        }
    }

    pthread_exit(NULL);
    // return NULL;
}

void *write_num(void *arg)
{
    while (1)
    {
        if (pthread_rwlock_trywrlock(&rwlock) == 0)
        {
            // do write
            rw_num++;
            printf("(write_%d): ----------------------------------------------------------------\n", *(int *)arg);

            // printf("(write_%d): wrtie num++, num = %d\n", *(int *)arg, rw_num);

            printf("(write_%d): ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n", *(int *)arg);
            pthread_rwlock_unlock(&rwlock);
        }
        usleep(1000);
    }

    pthread_exit(NULL);

    return NULL;
}

int main()
{
    pthread_t r_tid[READ_NUM], w_tid[WRITE_NUM];

    int r_thread_num[READ_NUM], w_thread_num[WRITE_NUM];

    for (int i = 0; i < READ_NUM; i++)
    {
        r_thread_num[i] = i;

        pthread_create(&r_tid[i], NULL, read_num, &r_thread_num[i]);

        pthread_detach(r_tid[i]);
    }

    for (int i = 0; i < WRITE_NUM; i++)
    {

        pthread_create(&w_tid[i], NULL, write_num, &(w_thread_num[i] = i));

        pthread_detach(w_tid[i]);
    }

    while(1)
    {
        sleep(10);
    }

    pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&rwlock);
    pthread_exit(NULL);

    return 0;
}