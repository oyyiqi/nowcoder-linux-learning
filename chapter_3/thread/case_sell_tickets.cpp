/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-03 10:34:57
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-03 10:42:37
 * @FilePath: /Linux/chapter_3/thread/case_sell_tickets.cpp
 * @Description: 多线程卖票
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

#define THREAD_NUM 5
#define TICKETS_NUM 1000

static int tickets_num = TICKETS_NUM;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *sell_tickets(void *arg)
{
    int *sell = new int(0);

    // with on mutex
    // while (tickets_num > 0)
    // {
    //     usleep(5000);
    //     printf("(child_%d): %d\n", *(int *)arg, tickets_num);
    //     (*sell)++;
    //     tickets_num--;
    // }

    // use mutex
    while (1)
    {

        // pthread_mutex_lock(&mutex);
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            if (tickets_num > 0)
            {
                printf("(child_%d): sell %d\n", *(int *)arg, tickets_num);
                tickets_num--;
                (*sell)++;
            }
            else
            {
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_mutex_unlock(&mutex);
        }
        usleep(1000);
    }

    printf("(child_%d): total=%d\n", *(int *)arg, *sell);

    pthread_exit(sell);
    return NULL;
}

int main()
{
    // create mutex
    // pthread_mutex_init(&mutex, NULL);

    // create child thread
    pthread_t tids[THREAD_NUM];
    int args[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++)
    {
        args[i] = i;
        int ret = pthread_create(&tids[i], NULL, sell_tickets, &args[i]);
        if (ret == -1)
        {
            perror("pthread_create");
            exit(0);
        }
    }

    // set thread detach or set join
    // for (int i = 0; i < THREAD_NUM; i++)
    // {
    //     pthread_detach(tids[i]);
    // }

    // set join
    int *real_sell[THREAD_NUM];
    int sum = 0;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(tids[i], (void **)&real_sell[i]);
        sum += *real_sell[i];
    }
    cout << "(main):" << sum << endl;

    // free resource
    for (int i = 0; i < THREAD_NUM; i++)
    {
        free(real_sell[i]);
    }
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);         // 防止return退出进程

    return 0;
}