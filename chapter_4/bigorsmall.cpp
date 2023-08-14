/***
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-08-04 16:49:47
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-04 17:25:57
 * @FilePath: /Linux/chapter_4/bigorset.cpp
 * @Description:
 ***/
#include <iostream>

using namespace std;

union bigorsmall
{
    int a;
    char b[sizeof(int)];
};

int main()
{
    union bigorsmall ts;
    ts.a = 0x12345678;

    for (int i = 0; i < sizeof(int); i++)
    {
        if (ts.b[i] == 0x12)
        {
            printf("b[%d] = 0x12\n", i);
        }

        if (ts.b[i] == 0x34)
        {
            printf("b[%d] = 0x34\n", i);
        }
        if (ts.b[i] == 0x56)
        {
            printf("b[%d] = 0x56\n", i);
        }
        if (ts.b[i] == 0x78)
        {
            printf("b[%d] = 0x78\n", i);
        }
    }


    return 0;
}