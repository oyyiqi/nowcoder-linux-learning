// Created by ouyangyiqi on 23-8-9.
#include "process_pool.h"
#include "CGI_serveer.h"


void cgi_conn::init(int epollfd, int sockfd, const sockaddr_in &client_addr)
{
    m_epollfd = epollfd;
    m_sockfd = sockfd;
    m_addr = client_addr;
    memset(m_buf, '\0', BUFFER_SIZE);
    m_read_index = 0;
}


int main()
{
    unsigned short port = 8080;
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd > 0);
    int ret = 0;
    struct sockaddr_in addr;
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(listenfd, (sockaddr *)&addr, sizeof(addr));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    processpool<cgi_conn> *pool = processpool<cgi_conn>::create(listenfd);

    if(pool)
    {
        pool->run();
        delete pool;
    }
    close(listenfd);

}

