/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-30 13:51:28
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-07-30 14:31:03
 * @FilePath: /Linux/mmapTest/copy_with_mmap.cpp
 * @Description: use memory map realize file copy
 ***/

/***使用内存映射实现文件拷贝功能
 *
 *
 *
 *
 *
 ***/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
using namespace std;

int main()
{
    int fd = open("source.txt", O_RDWR, 0664);
    if (fd == -1)
    {
        perror("open");
        exit(0);
    }
    int length = lseek(fd, 0, SEEK_END);

    int fd_copy = open("copy.txt", O_RDWR | O_CREAT, 0664);
    if (fd_copy == -1)
    {
        perror("open");
        exit(0);
    }
    int ret = truncate("./copy.txt", length);
    if (ret == -1)
    {
        perror("truncate");
        exit(0);
    }
     write(fd_copy, " ", 1);
    auto ptr_1 = mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    auto ptr_copy = mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd_copy, 0);
    memcpy(ptr_copy, ptr_1, length);

    munmap(ptr_1, length);
    munmap(ptr_copy, length);

    close(fd_copy);
    close(fd);

    return 0;
}
