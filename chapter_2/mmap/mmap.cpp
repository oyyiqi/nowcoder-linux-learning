/*
 * @Description: 内存映射
 * @Author: OuYang YIQI
 * @Date: 2023-07-28 10:52:07
 * @LastEditTime: 2023-07-30 13:15:35
 * @FilePath: /Linux/mmapTest/mmap.cpp
 */
/*
    #include <sys/mman.h>

    void *mmap(void *addr, size_t length, int prot, int flags int fd, off_t offset);
        1.功能:将一个文件或者设备的数据映射到内存中
        2.参数:
            - void *addr : NULL(由内核选择合适映射的虚拟内存区域)
            - size_t length : 要映射的数据的长度(不能为0)，建议使用文件的长度(stat,lseek)
                            【实际会映射页大小的整数倍，但是访问超过Length的地址依旧是非法的】
            - int prot : 映射区的操作权限设置（**如果要操作映射必须要有读权限**）
                -- PROT_EXEC : 可执行
                -- PROT_READ : 可读
                -- PROT_WRITE : 可写
                -- PROT_NONE : 没有权限
            - int flags:
                -- MAP_SHARED : 共享模式，内存和文件中的数据会自动进行同步，通常在需要进程通信时设置
                -- MAP_PRIVATE : 独占模式，不会自动同步，修改内存映射区数据时，不会修改原来的文件，而是创建一个新的文件
                -- MAP_ANONYMOUS : 匿名映射，此时 fd 应设为-1，offset设为0
            - int fd : 需要映射的文件的文件描述符(通过open得到，**prot权限必须在open权限范围之内**)
            - off_t offset : 偏移量(一般不用)，必须指定为4k(页大小)的整数倍，0表示不便宜
        3.返回值:
            -成功 : 返回创建的映射区的首地址
            -失败 ： MAP_FAILED ((void *) -1)

    **mmap哪些情况下会调用失败**
        1.参数 length = 0 时
        2.参数 prot 没有读权限，或者 prot 的权限 >fd open的权限
        3.偏移量不是页大小的整数倍

    int munmap(void *addr, size_t length);
        1.功能 : 释放内存映射区
        2.参数 :
            - void * addr : 要释放的内存首地址
            - size_t length : 要释放的内存大小
        3.返回值 ：
            成功 ：返回 0
            失败 ：返回 -1

    使用内存映射实现进程间通信：
        1.有关系的进程（父子进程）
            - 还没有子进程的时候
                -通过唯一的父进程，先创建内存映射区
            - 有了内存映射区以后，创建子进程
                -父子进程共享创建的内存映射区
        2.没有关系的进程间通信
            - 准备一个大小不为0的磁盘文件
            - 进程1通过磁盘文件创建内存映射区
                - 得到一个操作这块内存的指针
            - 进程2通过磁盘文件创建内存映射区
                - 得到一个操作这块内存的指针
            - 使用内存映射区通信

        注意：内存映射区通信，是非阻塞。

*/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <string.h>
using namespace std;

int main()
{
    int fd = open("./mmfile.txt", O_RDWR);
    if (fd == -1)
    {
        perror("open");
        exit(0);
    }

    int size = lseek(fd, 0, SEEK_END); // get file size
    auto addr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("map");
        exit(0);
    }
    
    pid_t pid = fork();
    
    if(pid > 0)
    {
        // parent : do read
        wait(0);
        cout << "recycle child" << endl;
        char *buf;
        strcpy(buf, (char *) addr);
        cout << "read data : " << buf << endl;
    }
    else if (pid == 0)
    {
        // child : do write
        char *buf = (char *) addr;
        strcpy(buf, "hello, ouyangyiqi");
    }
    else
    {
        perror("fork");
        exit(0);
    }
    
    return 0;
}