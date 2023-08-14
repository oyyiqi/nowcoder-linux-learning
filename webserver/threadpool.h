/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-10 09:10:19
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-10 10:04:38
 * @FilePath: /Linux/chapter_5/threadpool.h
 * @Description: 半同步半反应堆线程池实现
 ***/
#ifndef WEBSERVER_THREADPOOL_H
#define WEBSERVER_THREADPOOL_H

/** include **/

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "locker.h"
using namespace std;
template <typename T>
class threadpool
{
    int m_thread_number;   // 线程池中的线程数
    int m_max_requsts;     // 请求队列中允许的最大请求数
    pthread_t *m_threads;  // 描述线程池的数组，大小为 m_thread_number
    list<T*> m_workqueue;  // 请求队列
    locker m_queuelocker;  // 保护请求队列的互斥锁
    sem m_queuestat;       // 是否有任务需要处理
    bool m_stop;           // 是否结束线程

public:
    threadpool(int thread_number = 8, int max_requests = 10000);
    ~threadpool();
    bool append(T *request);

private:
    static void *worker(void *arg);
    void run();
};

template <typename T>
threadpool<T>::threadpool(int thread_number, int max_requests) : m_thread_number(thread_number), m_max_requsts(max_requests)
{
    if ((thread_number <= 0) || (max_requests <= 0))
    {
        throw std::exception();
    }

    m_threads = new pthread_t[m_thread_number];
    if (!m_threads)
    {
        throw std::exception();
    }

    for (int i = 0; i < thread_number; i++)
    {
        cout << "create the " << i << "th thread~" << endl;
        if (pthread_create(m_threads + i, NULL, worker, this) != 0)
        {
            delete[] m_threads;
            throw std::exception();
        }
        if (pthread_detach(m_threads[i]))
        {
            delete[] m_threads;
            throw std::exception();
        }
    }
}

template<typename T>
threadpool<T>::~threadpool()
{
    delete []m_threads;
    m_stop = true;
}

template <typename T>
bool threadpool<T>::append(T *request)
{
    m_queuelocker.lock();
    if (m_workqueue.size() > m_max_requsts)
    {
        m_queuelocker.unlock();
        return false;
    }

    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

template<typename T>
void *threadpool<T>::worker(void *arg)
{
    threadpool *pool = (threadpool *)arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run()
{
    while(!m_stop)
    {
        m_queuestat.wait();
        m_queuelocker.lock();
        if(m_workqueue.empty())
        {
            m_queuelocker.unlock();
            continue;
        }
        T *request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if(!request)
        {
            continue;
        }
        request->process();
    }
}

#endif //WEBSERVER_THREADPOOL_H
