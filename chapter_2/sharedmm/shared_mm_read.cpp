/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-01 14:41:55
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-01 14:55:35
 * @FilePath: /Linux/Sharedmm/shared_mm_read.cpp
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
#include <sys/shm.h>
#include <sys/pci.h>
using namespace std;

int main()
{
    // get shared mm
    int id = shmget(100, 0, IPC_CREAT);

    cout << "get shared mm : " << id << endl;
    
    // linked to the shared mm
    auto addr = shmat(id, NULL, 0);
    cout << "addr : " << addr << endl;
    // read to shared mm
    cout << (char *) addr << endl;
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
