/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-20 11:05:06
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-02 16:24:26
 * @FilePath: /Linux/ProcessTest/fork.cpp
 * @Description: fork()函数的使用
 ***/
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

/***
 * @description: 读时共享，写时拷贝机制
 * @return {*}
 ***/
void copy_on_write()
{
    static int s_num = 0;

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        cout << "           child s_num = " << s_num << "  &s_num = " << &s_num << endl;
        while (1)
        {
            sleep(2);
            cout << "           child s_num = " << s_num++ << "  &s_num = " << &s_num << endl;
        }
    }

    if (pid > 0)
    {
        while (1)
        {
            cout << "parent s_num = " << s_num++ << " &s_num = " << &s_num << endl;
            sleep(1);
        }
    }
}

int main()
{
    copy_on_write();
    return 0;
}