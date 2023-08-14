#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 4096
#define BACK_LOG 5

enum CHECK_STATE
{
    CHECK_STATE_REQUESTLINE = 0, /*当前正在分析请求行*/
    CHECK_STATE_HEADER           /*当前正在分析头部字段*/
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
    FORBIDDEN_REQUEST, /*客户端对请求的资源没有权限*/
    INTERNAL_ERROR,    /*服务器内部错误*/
    CLOSED_CONNECTION  /*客户端关闭连接*/
};

static const char *szret[] = {"I get a correct result\n", "Something wrong\n"};

/***
 * @description: 从状态机:用于解析出一行内容
 * @param {char} *buffer:应用程序的读缓冲区
 * @param {int} &checked_index:指向buffer中正在分析的字节
 * @param {int} &read_index:指向buffer中数据尾部的下一字节
 * @return {*}
 ***/
LINE_STATUS parse_line(char *buffer, int &checked_index, int &read_index)
{
    char temp;

    for (; checked_index < read_index; ++checked_index)
    {
        temp = buffer[checked_index];
        if (temp == '\r')
        {
            if (checked_index + 1 == read_index)
            {
                return LINE_OPEN;
            }
            else if (buffer[checked_index + 1] == '\n')
            {
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if (temp == '\n')
        {
            if ((checked_index > 1) && buffer[checked_index - 1] == '\r')
            {
                buffer[checked_index - 1] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

/***
 * @description: 解析请求行
 * @param {char} *temp ： 请求行字符串
 * @param {CHECK_STATE} &checkstate ： 当前状态
 * @return {HTTP_CODE} ： 请求行是否违法等标志位
 ***/
HTTP_CODE parse_requestline(char *temp, CHECK_STATE &checkstate)
{
    char *url = strpbrk(temp, " \t"); // 查找空字符和‘\t’
    if (!url)
    {
        return BAD_REQUEST;
    }

    *url++ = '\0';
    char *method = temp;
    if (strcasecmp(method, "GET") == 0) /*只支持GET方法*/
    {
        cout << "The request method is GET" << endl;
    }
    else
    {
        return BAD_REQUEST;
    }

    url += strspn(url, " \t");
    char *version = strpbrk(url, " \t"); //???
    if (!version)
    {
        return BAD_REQUEST;
    }

    *version++ = '\0';
    version += strspn(version, " \t");

    if (strcasecmp(version, "HTTP/1.1") != 0) /*only http/1.1*/
    {
        return BAD_REQUEST;
    }

    /*检查url是否合法*/
    if (strncasecmp(url, "http://", 7) == 0)
    {
        url += 7;
        url = strchr(url, '/');
    }

    if (!url || url[0] != '/')
    {
        return BAD_REQUEST;
    }

    checkstate = CHECK_STATE_HEADER;

    return NO_REQUEST;
}

/***
 * @description: 解析http头部字段
 * @param {char} *temp ： 请求头字符串（一行）
 * @return {HTTP_CODE} 请求状态标志
 ***/
HTTP_CODE parse_headers(char *temp)
{
    if (temp[0] == '\0')
    {
        return GET_REQUEST;
    }

    else if (strncasecmp(temp, "Host:", 5) == 0)
    {
        temp += 5;
        temp += strspn(temp, " \t");
        cout << "the request host is: " << temp << endl;
    }

    else
    {
        cout << "I can not handle this header" << endl;
    }

    return NO_REQUEST;
}

/***
 * @description: 解析http请求的入口函数
 * @param {char} *buffer ： 保存请求信息的字符串数组
 * @param {int} &checked_index ： 当前处理的字节
 * @param {CHECK_STATE} &checkstate ： 当前状态
 * @param {int} &read_index ：
 * @param {int} &start_line ：
 * @return {HTTP_CODE}
 ***/
HTTP_CODE parse_content(char *buffer, int &checked_index, CHECK_STATE &checkstate, int &read_index, int &start_line)
{
    LINE_STATUS linestatus = LINE_OK;
    HTTP_CODE retcode;
    while ((linestatus = parse_line(buffer, checked_index, read_index)) == LINE_OK)
    {
        char *temp = buffer + start_line;
        start_line = checked_index;

        switch (checkstate)
        {
        case CHECK_STATE_REQUESTLINE:
            retcode = parse_requestline(temp, checkstate);
            if (retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            break;
        case CHECK_STATE_HEADER:
        {
            retcode = parse_headers(temp);
            if (retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if (retcode == GET_REQUEST)
            {
                return GET_REQUEST;
            }
            break;
        }
        
        default:
            return INTERNAL_ERROR;
        }
    }
}

int main(int argc, char *argv[])
{

    //const char ip[] = "7.249.5.141";
    unsigned short port = 8080;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    int ret = bind(listenfd, (sockaddr *)&addr, sizeof(addr));
    assert(ret != -1);
    ret = listen(listenfd, BACK_LOG);
    assert(ret != -1);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int clientfd = accept(listenfd, (sockaddr *)&client_addr, &client_addr_len);
    if (clientfd == -1)
    {
        perror("accept");
        close(listenfd);
        exit(-1);
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);

    int read_len = 0;
    int read_index = 0;
    int checked_index = 0;
    int start_line = 0;

    /*set init status*/
    CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;

    while (1)
    {
        read_len = recv(clientfd, buffer + read_index, BUFFER_SIZE - read_index, 0);
        cout << "[RECEIVE]" << buffer << endl;
        if (read_len == -1)
        {
            perror("recv");
            break;
        }

        else if (read_len == 0)
        {
            cout << "remote client disconnected" << endl;
            break;
        }

        read_index += read_len;

        HTTP_CODE result = parse_content(buffer, checked_index,
                                         checkstate, read_index, start_line);
        if(result == NO_REQUEST)
        {
            continue;
        }

        else if(result == GET_REQUEST)
        {
            send(clientfd, szret[0], strlen(szret[0]), 0);
            cout << "OK_REQUEST" << endl;
            break;
        }

        else
        {
            send(clientfd, szret[1], strlen(szret[1]), 0);
            cout << "BAD_REQUEST" << endl;
            break;
        }
    }
    close(clientfd);
    close(listenfd);
    return 0;
}
