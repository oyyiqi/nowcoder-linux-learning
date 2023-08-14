/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-01 10:14:10
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-01 14:42:13
 * @FilePath: /Linux/Sharedmm/shared_mm.cpp
 * @Description:
 ***/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;

int main()
{
    // create shared mm
    int id = shmget(100, 1024, IPC_CREAT | IPC_EXCL | 0666);

    cout << "create shared mm : " << id << endl;

    // linked to the shared mm
    auto addr = shmat(id, NULL, 0);
    cout << "addr : " << addr << endl;
    // write to shared mm
    char str[] = "hello world";
    memcpy(addr, str, sizeof(str));
    cout << "sizeof(str):" << sizeof(str) << endl;

    getchar();  
    // unlinked shared mm
    int ret = shmdt(addr);
    if (ret == -1)
    {
        perror("shmdt");
    }
    // close shared mm
    shmctl(id, IPC_RMID, NULL);

    return 0;
}
