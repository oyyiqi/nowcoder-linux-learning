#ifndef WEBSERVER_HTTP_CONN_H
#define WEBSERVER_HTTP_CONN_H
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
#include <errno.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/uio.h>
#include "locker.h"

static const int FILENAME_LEN = 200;
static const int READ_BUFFER_SIZE = 2048;
static const int WRITE_BUFFER_SIZE = 1024;
enum METHOD
{
    GET,
    POST,
    HEAD,
    PUT,
    DELETE,
    TRACE,
    OPTIONS,
    CONNECT,
    PATCH
};

enum CHECK_STATE
{
    CHECK_STATE_REQUESTLINE = 0,
    CHECK_STATE_HEADER,
    CHECK_STATE_CONTENT
};

enum LINE_STATUS
{
    LINE_OK,  /*读取到一个完整的行*/
    LINE_BAD, /*行出错*/
    LINE_OPEN /*行数据不完整*/
};

enum HTTP_CODE
{
    NO_REQUEST,        /*请求不完整*/
    GET_REQUEST,       /*完整的客户请求*/
    BAD_REQUEST,       /*客户端请求错误*/
    NO_RESOURCE,       /*没有资源*/
    FORBIDDEN_REQUEST, /*客户端对请求的资源没有权限*/
    FILE_REQUEST,      /*文件请求*/
    INTERNAL_ERROR,    /*服务器内部错误*/
    CLOSED_CONNECTION, /*客户端关闭连接*/
};

class http_conn
{
    /*****************************************/
    /*           **成员变量定义**              */
    /*****************************************/

public:
    static int m_epollfd;    /**/
    static int m_user_count; /*统计用户数量*/

private:
    /*****************************************/
    /*   该Http连接的socket和对方的socket地址   */
    /*****************************************/
    int m_sockfd;
    sockaddr_in m_address;

    /*****************************************/
    /*              缓冲区相关定义             */
    /*****************************************/
    char m_read_buf[READ_BUFFER_SIZE];      /* 读缓冲区 */
    char m_write_buf[WRITE_BUFFER_SIZE];    /* 写缓冲区 */
    int m_read_idx;                         /* 标识读缓冲区中已经读入的客户数据的最后一个字节的下一个位置 */
    int m_checked_idx;                      /* 当前正在分析的字节在缓冲区中的位置 */
    int m_start_line;                       /* 当前行在缓冲区中的起始位置 */
    int m_write_idx;                        /* 写缓冲区中待发送的字节数 */

    /*****************************************/
    /*         解析到的Http内容相关定义         */
    /*****************************************/
    CHECK_STATE m_check_state;      /* 主状态机当前的状态 */
    METHOD m_method;                /* 请求方法 */
    char m_real_file[FILENAME_LEN]; /* 客户端请求的文件的完整路径 */
    char *m_url;                    /* 客户端请求的目标文件的文件名 */
    char *m_host;                   /* 主机名(域名) */
    char *m_version;                 /* Http协议版本号 */
    int m_conten_length;            /* Http请求的消息体长度 */
    bool m_linger;                  /* 是否保持长连接 */

    /*****************************************/
    /*               其他定义                 */
    /*****************************************/
    char *m_file_address;    /* 客户请求的目标文件被映射到内存的起始地址 */
    struct stat m_file_stat; /* 目标文件的状态：是否存在、是否可读等等 */
    struct iovec m_iv[2];
    int m_iv_count;          /* 被写内存块的数量 */

    /*****************************************/
    /*           **成员函数定义**              */
    /*****************************************/
public:
    void init_conn(int sockfd, const sockaddr_in &addr); /* 初始化新接受的连接 */
    void close_conn(bool real_close = true);             /* 关闭连接 */
    void process();                                      /* 处理用户请求 */
    bool read();                                         /* 非阻塞读操作 */
    bool write();                                        /* 非阻塞写操作 */

private:
    void init();                            /* 初始化连接 */
    HTTP_CODE process_read();               /* 解析HTTP请求 */
    bool process_write(HTTP_CODE ret);      /* 填充HTTP响应 */

    /*****************************************/
    /*          解析HTTP请求相关函数            */
    /*****************************************/
    HTTP_CODE parse_request_line(char *text);
    HTTP_CODE parse_headers(char *text);
    HTTP_CODE parse_content(char *text);
    HTTP_CODE do_request();
    char *get_line() { return m_read_buf + m_start_line; }
    LINE_STATUS parse_line();

    /*****************************************/
    /*          填充HTTP响应相关函数            */
    /*****************************************/
    void unmap();
    bool add_response(const char *format, ...);
    bool add_content(const char *content);
    bool add_status_line(int status, const char *title);
    bool add_headers(int content_length);
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_line();
};

#endif // WEBSERVER_HTTP_CONN_H
