/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-02 08:30:20
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-02 10:21:08
 * @FilePath: /Linux/Deamon/deamon.cpp
 * @Description: 守护进程
 ***/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

void alarm(int sig)
{
    time_t tm = time(NULL);
    auto localtm = localtime(&tm);
    char *asctm = asctime(localtm);
    cout << "hello I am deamon : " << getpid() << "," << asctm << endl;
}

// 创建一个守护进程，每隔2s获取一次系统时间，并写到磁盘文件中
int main()
{
    // create child process
    pid_t pid = fork();

    // exit parent process
    if (pid > 0)
    {
        exit(0);
    }

    // set session id for child

    setsid(); // 脱离原来的会话，调用者成为新的会话首进程，以及新的进程组首进程，新的会话id以及新的组id都是调用者的进程pid

    // set mask
    umask(0);

    // change workspace
    chdir("/home/ouyangyiqi/");

    // close or redirect file descriptOUT
    close(STDIN_FILENO);
    close(STDERR_FILENO);
    // close(STDOUT_FILENO);
    int fd = open("/home/ouyangyiqi/test.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
    dup2(fd, STDOUT_FILENO);
    // do some thing

    // 1. create timer
    struct itimerval val;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;
    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &val, NULL);

    // 2.catch SIGALRM
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = alarm;

    sigaction(SIGALRM, &act, NULL);

    while (1)
    {
        sleep(1);
    }

    return 0;
}