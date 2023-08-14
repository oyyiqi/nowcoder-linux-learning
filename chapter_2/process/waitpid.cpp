/*
 * @Description: waitpid test
 * @Author: OuYang YIQI
 * @Date: 2023-07-26 22:19:45
 * @LastEditTime: 2023-07-27 16:28:30
 * @FilePath: /Linux/ProcessTest/waitpid.cpp
 */

/*
    #include <sys/types.h>
    #include <sys/wait.h>
    pid_t waitpid(pid_t pid, int *wstatus, int options);
        功能:回收指定进程号的子进程，可以设置是否阻塞
        参数:
            - pid:
                pid > 0 : 回收进程号为pid的子进程
                pid = 0 : 回收当前进程组的任意一个子进程
                pid = -1 : 回收任意一个子进程，相当于wait()
                pid < -1 : 回收组id = |pid|中的任意一个进程
            - options:设置调用者是否阻塞
                0 : 阻塞
                WNOHANG(宏) : 非阻塞
        返回值:
            > 0 : 回收的子进程ID
            = 0 : options=WNOHANG，说明还有子进程存在
            =-1 : 错误，或者没有进程可以回收了
        说明 :
            1. waitpid(pid, NULL, WNOHANG); 
                检查当前有没有属于pid的僵尸进程，有则回收，并返回回收的子进程的pid；
                如果当前有属于pid的进程，但是其中没有僵尸进程，则返回0;
                如果没有属于pid的进程，则返回-1
            2. waitpid(pid, NULL, 0);
                成功 : 返回 pid
                失败 : 返回 -1
*/
#include <iostream>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <wait.h>

int addtest(int a, int b)
{
    return a + b;
}
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
            sleep(2);
            int st;
            int ret = waitpid(-1, &st, WNOHANG);
            if (ret == -1)
            {
                cout << "all child process resource recycled" << endl;
                break;
            }

            else if (ret == 0)
            {
                // 还存在子进程
                cout << "current iterator no collect child proccess, but still exit child proccess" << endl;
                continue;
            }
            else if (ret > 0)
            {
                if (WIFEXITED(st))
                {
                    // is normal exit
                    cout << "exit code is " << WEXITSTATUS(st) << endl;
                }

                if (WIFSIGNALED(st))
                {
                    // is unnormal shut down
                    cout << "shut down by signal " << WTERMSIG(st) << endl;
                }
                cout << "child process shut down, pid = " << ret << endl;
            }
        }
    }
    else if (pid == 0)
    {
        // child proccess
        while (1)
        {
            sleep(2);
            cout << "child, pid = " << getpid() << endl;
        }
        exit(0);
    }

    return 0;
}
