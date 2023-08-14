// Created by ouyangyiqi on 23-8-9.

#ifndef CHAPTER_5_PROCESS_POOL_H
#define CHAPTER_5_PROCESS_POOL_H

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
#include <semaphore.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <assert.h>

#define PROCESS_NUMBER 8
using namespace std;

class process
{
public:
    process() : m_pid(-1) {}
    pid_t m_pid;
    int m_pipefd[2];
};

template <typename T>
class processpool
{
private:
    processpool(int listenfd, int process_number = PROCESS_NUMBER);

public:
    /*单例模式*/
    static processpool<T> *create(int listenfd, int process_number = PROCESS_NUMBER)
    {
        if (!m_instence)
        {
            m_instence = new processpool<T>(listenfd, process_number);
        }
        return m_instence;
    }
    ~processpool(){delete[] m_sub_process};

    void run();

private:
    void setup_sig_pipe();
    void run_parent();
    void run_child();

private:
    static const int MAX_PROCESS_NUMBER = 16;  // 进程池允许的最大子进程数量
    static const int USER_PER_PROCESS = 65536; // 每个子进程最多能处理的客户数量
    static const int MAX_EVENT_NUMBER = 10000; // epoll最多能处理的事件数
    int m_process_number;                      // 进程池中的进程总数
    int m_idx;                                 // 子进程在池中的序号
    int m_epollfd;                             // 每个进程都有一个epoll内核事件表
    int m_listenfd;                            // 监听socket
    int m_stop;                                // 子进程通过m_stop来决定是否停止运行
    process *m_sub_process;                    // 保存所有子进程的描述信息
    static processpool<T> *m_instance;         // 进程池静态实例
};




#endif //CHAPTER_5_PROCESS_POLL_H
