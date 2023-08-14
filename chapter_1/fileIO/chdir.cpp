#include<unistd.h>
#include<stdio.h>
#include<iostream>
using namespace std;

int main()
{
    char buf[128];
    getcwd(buf, sizeof(buf));
    cout << "当前工作路径为：" << buf << endl;

    // 修改工作路径
    int ret = chdir("../GDBTest");
    if(ret == -1)
    {
        perror("chdir");
        return -1;
    }
    
    char buf1[128];
    getcwd(buf1, sizeof(buf1));
    cout << "当前工作路径为：" << buf1 << endl;
    
}