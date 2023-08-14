/*** 
 * @Author: oyyiqi 2427268454@qq.com
 * @Date: 2023-07-21 21:45:09
 * @LastEditors: oyyiqi 2427268454@qq.com
 * @LastEditTime: 2023-08-02 15:13:04
 * @FilePath: /Linux/ProcessTest/exit.cpp
 * @Description: exit()与_exit()的区别
 ***/
#include<iostream>
#include<unistd.h>

using namespace std;

int main()
{
    cout << "hello" << endl;
    cout << "word" ;
    // _exit(0);    //退出之后缓冲区没有刷新，因此不会输出word
    exit(0);        //exit()会刷新缓冲区，因此会输出word
    cout << "word" << endl;
    system("pause");
    return 0;
}

