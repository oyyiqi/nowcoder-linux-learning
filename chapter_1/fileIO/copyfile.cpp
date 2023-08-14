/*
    #include <unistd.h>
    ssize_t read(int fd, void *buf, size_t count);
        参数：
            - fd:文件描述符（通过open得到）
            - buf(传出参数):读取的数据存放的地方
            - count:指定的数组大小
        返回值：
            - 成功:
                >0:返回实际读取到的字节数
                =0:文件已经读取完了
            - 失败:返回-1，并设置errno
    size_t write(int fd, const void *buf, size_t count);
        参数：
            - fd:文件描述符，通过open得到
            - buf:要往磁盘写入的数据
            - count:实际要写入数据的大小
        返回值:
            - 成功:返回实际写入的字节数
            - 失败:返回-1，并设置errno

    
*/

#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
    // 1.打开源文件
    int src_fd = open("a.txt", O_RDONLY);
    if(src_fd == -1)
    {
        perror("open faild");
        return -1;
    }

    // 2.创建拷贝文件
    int dst_fd = open("b.txt", O_WRONLY | O_CREAT, 777);
    if(dst_fd == -1)
    {
        perror("open or create error");
        return -1;
    }

    // 3.文件读写
    char buf[1024] = {0};
    int len = 0;
    while((len = read(src_fd, buf, sizeof(buf))) > 0)
    {
        write(dst_fd, buf, len);
    }

    // 4.关闭文件   
    close(src_fd);
    close(dst_fd);
    return 0;

    
    
}