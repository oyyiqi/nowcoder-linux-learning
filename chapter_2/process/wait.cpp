/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-26 16:47:20
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-01 09:42:44
 * @FilePath: /Linux/ProcessTest/wait.cpp
 * @Description: 
 ***/

/*
    #include <sys/types.h>
    #include <sys/wait.h>

    pid_t wait(int *wstatus);
        - 功能 : 自动分析是否有自己的子进程处于退出状态(僵尸进程),有的话回收这个子进程的资源，然后返回，否则将一直阻塞，直到有子进程结束。
        - 参数:
            - int* wastatus:进程退出时的状态信息，传入的是一个int类型的地址，该参数是一个传出参数
        - 返回值:
            - 成功:返回被回收的子进程的id
            - 失败:-1(所有子进程都已经结束，或者函数调用失败时)
        - 说明:调用wait函数的进程会被挂起（阻塞）
    pid_t waitpid(pid_t pid, int *wstatus, int options);
    int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
*/
#include <iostream>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <wait.h>
using namespace std;
int main()
{
    pid_t pid;
    for (int i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            break;
        }
    }

    if (pid > 0)
    {
        while (1)
        {
            printf("parent, pid = %d\n", getpid());
            int ret = wait(NULL);
            if (ret == -1)
            {
                cout << "all child process resource recycled" << endl;
                break;
            }
            cout << "child process shut down, pid = " << ret << endl;
            sleep(1);
        }
    }
    else if (pid == 0)
    {
        // child proccess
        while (1)
        {
            sleep(1);
            cout << "child, pid = " << getpid() << endl;
        }
    }

    return 0;
}
