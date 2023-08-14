#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;
/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    int open(const char *pathname, int flags, mode_t mode);
        参数：
            - pathname:要打开的文件路径；
            - flag:对文 件的操作权限等设置
                必选：O_RDONLY O_WRONLY O_RDWR 这三个设置是互斥的
                可选：O_CREAT (文件不存在则创建)，O_APPEND等
            - mode(可选): 权限 如:777
        返回值：
            返回一个新的文件描述符，如果调用失败，返回-1；

    errno:属于Linux系统函数库里面的一个全局变量，记录的是最近的错误号；

    #include<stdio.h>

    void perror(const char *s); //打印errno对应的错误描述
        参数：
            - s:用户描述
*/


int main()
{
    int fd = open("a.txt", O_RDWR | O_CREAT, 0777);  // 读写，不存在则创建，权限777
    if(fd == -1)
    {
        perror("open");
    } 
    //读写操作


    close(fd);

    return 0;
}
