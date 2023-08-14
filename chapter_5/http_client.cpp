#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sstream>
using namespace std;

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
    inet_pton(AF_INET, "192.168.1.103", &server_sock.sin_addr.s_addr);
    server_sock.sin_port = htons(8888);

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
        stringstream request;
        request << "GET /index.html HTTP/1.1\r\n";
        request << "Host: 192.168.1.103:8888\r\n";
        request << "User-Agent: MyHttpClient\r\n";
        request << "Proxy-Connection: keep-alive\r\n"; // 表示请求后关闭连接
        request << "\r\n";                  // 空行表示请求头结束，内容开始
        string requeststr = request.str();
        int ret = write(fd, requeststr.c_str(), requeststr.length());
        if (ret == -1)
        {
            perror("write");
            exit(-1);
        }
        cout << "[client--->server]:" << requeststr << endl;
        
        char recv_buf[1024] = {0};
        int len = read(fd, recv_buf, sizeof(recv_buf));
        if (len <= 0)
        {
            perror("read");
            exit(-1);
        }
        cout << "[server--->client]:" << recv_buf << endl;
        // memset(recv_buf, 0, sizeof(recv_buf));
    }
    return 0;
}