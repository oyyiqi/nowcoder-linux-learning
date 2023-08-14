#ifndef CHAPTER_5_CGI_SERVEER_H
#define CHAPTER_5_CGI_SERVEER_H

#include <arpa/inet.h>

class cgi_conn
{
public:
    cgi_conn();
    ~cgi_conn();
    void init(int epollfd, int sockfd, const sockaddr_in &client_addr);
    void process();
private:
    static const int BUFFER_SIZE =1024;
    static int m_epollfd;
    int m_sockfd;
    sockaddr_in m_addr;
    char m_buf[BUFFER_SIZE];
    int m_read_index;   //标记读缓冲区中已经读入的客户数据的最后一个字节的下一个位置
};

int cgi_conn::m_epollfd = -1;


#endif // CHAPTER_5_CGI_SERVEER_H
