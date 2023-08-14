/*
 Created by ouyangyiqi 
*/
#include<iostream>
//#include<vector>

using namespace std;

#include<unistd.h>
#include<sys/types.h>
int main()
{
    pid_t pid = fork();
    if(pid > 0)
    {
        cout << " this is parent process, pid is :" << getpid() << endl;
    }

    if(pid == 0)
    {
        execl("/bin/ps", "ps", "aux", NULL);
        cout << " this is child process, pid is " << getpid() << endl; 
    }
    sleep(1);
    for(int i = 0; i < 10; i++)
    {
        cout<< i << " " << getpid() << endl;
    }
    return 0;
}
