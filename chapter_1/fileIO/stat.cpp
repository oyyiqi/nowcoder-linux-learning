/*
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
    int stat(const char *pathname,struct stat *statbuf);
        作用：获取一个文件相关的一些信息
        参数：
            -pathname:操作的文件路径
            -statbuf:结构体变量，传出参数，用于保存获取到的文件的信息
        返回值：
            成功：返回0
            失败：返回-1，并设置errno

    int lstat(const char *pathname,struct stat *statbuf);
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<iostream>
using namespace std;

int main()
{

    struct stat buf;
    int ret = stat("a.txt", &buf);
    if(ret == -1)
    {
        perror("stat");
        return -1;
    }
    cout << buf.st_gid << endl;
    return 0;

}