/*
Linux系统函数
    #include <sys/types.h>
    #include <unistd.h>
    off_t lseek(int fd,off_t offset,int whence);
        参数：
            - fd:文件描述符，通过open得到的，通过这个fd操作某个文件
            - offset:偏移量
            - whence:
                SEEK_SET:设置文件指针的偏移量
                SEEK_CUR:当前位置+第二个参数offset的值
                SEEK END:文件大小+第二个参数offset的值
        返回值:文件指针的位置
        作用：
            1.移动文件指针到文件头
                1seek(fd,0,SEEK_SET);

            2.获取当前文件指针的位置
                1seek(fd,0,SEEK_CUR);

            3.获取文件长度
                1seek(fd,0,SEEK_END);
            
            4.拓展文件长度(下载文件时，先占用空间)
                lseek(fd,len,SEEK_END);
                注意:需要在文件末写一次数据
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<iostream>
int main()
{
    int fd = open("a.txt", O_RDWR);
    if(fd == -1)
    {
        perror("open");
    }
    int current = lseek(fd, 0, SEEK_CUR);
    std::cout << current << std::endl;




    return 0;
}