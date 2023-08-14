#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
using namespace std;

#define SERVER_IP "192.168.1.103"

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(-1);
    }

    sockaddr_in server_sock;
    server_sock.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_sock.sin_addr.s_addr);
    server_sock.sin_port = htons(8080);

    cout << "connecting to server ..." << endl;
    sleep(1);
    int ret = connect(fd, (sockaddr *)&server_sock, sizeof(server_sock));
    if (ret == -1)
    {
        perror("connect");
        exit(-1);
    }
    cout << "success connect with server!" << endl;

    while (1)
    {
        cout << "[client--->server]:";
        string msg;
        getline(cin, msg);
        int ret = write(fd, msg.c_str(), msg.length()+1);
        msg = "";
        if(ret == -1)
        {
            perror("write");
            exit(-1);
        }
        char recv_buf[1024] = {0};
        int len = read(fd, recv_buf, sizeof(recv_buf));
        if(len <= 0)
        {
            perror("read");
            exit(-1);
        }
        cout << "[server--->client]:" << recv_buf << endl; 
        // memset(recv_buf, 0, sizeof(recv_buf));
    }

    return 0;
}