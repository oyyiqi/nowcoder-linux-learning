/*
 * @Description: pipe test
 * @Author: OuYang YIQI
 * @Date: 2023-07-27 17:26:46
 * @LastEditTime: 2023-07-27 22:49:44
 * @FilePath: /Linux/ProcessCommunication/pipe.cpp
 */

/*
    #include <unistd.h>

    struct fd_pair {
        long fd[2];
    };
    struct fd_pair pipe();

    int pipe(int pipefd[2]);
        功能：创建一个匿名管道，用于进程间通信
        参数：
            pipefd[2]:传出参数，其中pipefd[0]对应管道读端，pipefd[1]对应管道写端
        返回值：
            0：调用成功
            -1：调用失败
        注意：
            1.匿名管道只能用于具有关系之间的进程通信（父子进程，兄弟进程等）
            2.管道默认是阻塞的，如果管道中没有数据，read阻塞; 如果管道满了，write阻塞
        总结：
            读管道：
                管道中有数据，read返回实际读到的字节数。
                管道中无数据：
                    写端被全部关闭，read返回 0（相当于读到文件的末尾）
                    写端没有完全关闭，read阻塞等待
            写管道：
                管道读端全部被关闭，进程异常终止（进程收到SIGPIPE信号）
                管道读端没有全部关闭：
                    管道已满，write阻塞
                    管道没有满，write将数据写入，并返回实际写入的字节数
*/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int main()
{
    // create pipe before fork()
    int fd[2];
    int ret = pipe(fd);
    if (ret == -1)
    {
        perror("pipe");
    }

    // create child process
    pid_t pid = fork();
    if (pid > 0)
    {
        // parent process : receive message
        char buf[1024];
        int recvNum = 0;
        char msg[] = "hello, I am parent process";
        while (1)
        {
            int len = read(fd[0], buf, sizeof(buf));
            if (len > 0)
            {
                cout << "i have received message from pipe count:" << ++recvNum << endl;
            }
            else if (len == -1)
            {
                perror("read");
            }

            int ret = write(fd[1], msg, sizeof(msg));
            if(ret > 0)
            {
                cout << "parent process write to pipe success" << endl;
            }
            sleep(1);

        }
    }
    else if (pid == 0)
    {
        // child process : send message
        // sleep(10); // read process will block util pipe writed
        char msg[] = "hello, I am child process";
        int sendNum = 0;
        char buf[1024];
        while (1)
        {
            int ret = write(fd[1], msg, strlen(msg));
            if (ret > 0)
            {
                cout << "child process write pipe success, count :" << ++sendNum << endl;
            }
            else if (ret == -1)
            {
                perror("write");
            }
            
            sleep(1);
            
            int len = read(fd[0], buf, sizeof(buf));
            if(len > 0)
            {
                cout << "child receive : " << buf << endl;
            }
        }
    }

    return 0;
}