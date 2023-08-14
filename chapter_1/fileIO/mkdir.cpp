#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
    int ret = mkdir(argv[1], 0664);
    if(ret == -1)
    {
        perror("mkdir");
    }

    return 0;
}