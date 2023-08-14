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
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
using namespace std;

int main()
{

    // 将ip字符串转换成整数
    char src[] = "196.168.1.1";
    uint32_t num = 0;
    int ret = inet_pton(AF_INET, src, &num);

    unsigned char *p = (unsigned char *)&num;
    cout << "*p = " << (int)*p << endl;
    cout << "*(p+1) = " << (int)*(p + 1) << endl;
    cout << "*(p+2) = " << (int)*(p + 2) << endl;
    cout << "*(p+3) = " << (int)*(p + 3) << endl;

    //

    char ipstr[16];
    if (ipstr == inet_ntop(AF_INET, &num, ipstr, sizeof(ipstr)))
    {
        cout << "转换成功!" << endl;
        cout << "ipstr = " << ipstr << endl;
    }

    return 0;
}