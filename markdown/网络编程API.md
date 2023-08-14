# 1.字节序转换函数

在计算机中，字节序（endianness）指的是多字节数据在内存中的存储顺序。主要有两种字节序：大端字节序（Big-Endian）和小端字节序（Little-Endian）。在大端字节序中，高字节存储在低地址，低字节存储在高地址；而在小端字节序中，低字节存储在低地址，高字节存储在高地址。网络字节序通常采用大端字节序，而 x86 架构的计算机通常采用小端字节序。

在进行跨平台数据传输或处理时，经常需要进行字节序转换。以下是常用的字节序转换函数：

**1. `htons` 函数：**

```c
#include <arpa/inet.h>

uint16_t htons(uint16_t hostshort);
```

- 功能说明：`htons()` 函数用于将一个 16 位无符号整数（hostshort）从主机字节序转换为网络字节序（大端字节序）。
- 参数说明：`hostshort` 是一个 16 位无符号整数，表示要转换的值。
- 返回值：返回转换后的网络字节序（大端字节序）值。

**2. `htonl` 函数：**

```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);
```

- 功能说明：`htonl()` 函数用于将一个 32 位无符号整数（hostlong）从主机字节序转换为网络字节序（大端字节序）。
- 参数说明：`hostlong` 是一个 32 位无符号整数，表示要转换的值。
- 返回值：返回转换后的网络字节序（大端字节序）值。

**3. `ntohs` 函数：**

```c
#include <arpa/inet.h>

uint16_t ntohs(uint16_t netshort);
```

- 功能说明：`ntohs()` 函数用于将一个 16 位无符号整数（netshort）从网络字节序（大端字节序）转换为主机字节序。
- 参数说明：`netshort` 是一个 16 位无符号整数，表示要转换的值。
- 返回值：返回转换后的主机字节序值。

**4. `ntohl` 函数：**

```c
#include <arpa/inet.h>

uint32_t ntohl(uint32_t netlong);
```

- 功能说明：`ntohl()` 函数用于将一个 32 位无符号整数（netlong）从网络字节序（大端字节序）转换为主机字节序。
- 参数说明：`netlong` 是一个 32 位无符号整数，表示要转换的值。
- 返回值：返回转换后的主机字节序值。


------------------------------------------------------------------------------


# 2. ip转换函数


在C语言中，可以使用一些函数将IP地址字符串与网络字节序的二进制IP地址之间进行转换。IP地址通常表示为点分十进制（dotted-decimal）字符串格式（例如："192.168.1.1"），而网络字节序的二进制IP地址是一个32位整数。

以下是常用的IP地址转换函数：

**1. `inet_aton` 函数：**

```c
#include <arpa/inet.h>

int inet_aton(const char *cp, struct in_addr *inp);
```

- 功能说明：`inet_aton()` 函数用于将IP地址字符串转换为网络字节序的二进制IP地址。
- 参数说明：
  - `cp` 是一个指向以空字符结尾的IP地址字符串的指针，表示要转换的IP地址。
  - `inp` 是一个指向 `struct in_addr` 结构的指针，用于存储转换后的二进制IP地址。
- 返回值：如果转换成功，函数返回非零值（通常为1）。如果转换失败，函数返回零。

**2. `inet_addr` 函数：**

```c
#include <arpa/inet.h>

in_addr_t inet_addr(const char *cp);
```

- 功能说明：`inet_addr()` 函数用于将IP地址字符串转换为网络字节序的二进制IP地址。
- 参数说明：`cp` 是一个指向以空字符结尾的IP地址字符串的指针，表示要转换的IP地址。
- 返回值：如果转换成功，函数返回网络字节序的二进制IP地址。如果转换失败，函数返回 `INADDR_NONE`。

**3. `inet_ntoa` 函数：**

```c
#include <arpa/inet.h>

char *inet_ntoa(struct in_addr in);
```

- 功能说明：`inet_ntoa()` 函数用于将网络字节序的二进制IP地址转换为点分十进制字符串格式的IP地址。
- 参数说明：`in` 是一个 `struct in_addr` 结构，表示要转换的二进制IP地址。
- 返回值：返回指向表示转换后的点分十进制字符串格式的IP地址的指针。需要注意的是，函数返回的指针指向的是一个静态缓冲区，不是动态分配的内存。每次调用此函数都会覆盖之前的结果。

**4. `inet_pton` 函数：**

```c
#include <arpa/inet.h>

int inet_pton(int af, const char *src, void *dst);
```

- 功能说明：`inet_pton()` 函数用于将IP地址字符串转换为网络字节序的二进制IP地址，支持 IPv4 和 IPv6 地址。
- 参数说明：
  - `af` 是一个整数值，用于指定地址族（IPv4 或 IPv6）。可以使用 `AF_INET` 表示 IPv4，`AF_INET6` 表示 IPv6。
  - `src` 是一个指向以空字符结尾的IP地址字符串的指针，表示要转换的IP地址。
  - `dst` 是一个指向存储转换后的二进制IP地址的缓冲区的指针。
- 返回值：如果转换成功，函数返回 1。如果转换失败或提供的地址族不支持，函数返回 0。

**5. `inet_ntop` 函数：**

```c
#include <arpa/inet.h>

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
```

- 功能说明：`inet_ntop()` 函数用于将网络字节序的二进制IP地址转换为点分十进制字符串格式的IP地址，支持 IPv4 和 IPv6 地址。
- 参数说明：
  - `af` 是一个整数值，用于指定地址族（IPv4 或 IPv6）。可以使用 `AF_INET` 表示 IPv4，`AF_INET6` 表示 IPv6。
  - `src` 是一个指向存储网络字节序的二进制IP地址的缓冲区的指针。
  - `dst` 是一个指向存储转换后的点分十进制字符串格式的IP地址的缓冲区的指针。
  - `size` 是缓冲区 `dst` 的大小。
- 返回值：如果转换成功，函数返回指向表示转换后的点分十进制字符串格式的IP地址的指针。如果转换失败或提供的地址族不支持，函数返回 `NULL`。


# 3. Socket函数

在C语言中，套接字（socket）函数用于进行网络通信。套接字是在网络上进行通信的接口，可以在客户端和服务器之间传递数据。以下是常用的套接字函数：

**1. `socket` 函数：**

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

- 功能说明：`socket()` 函数用于创建一个套接字，返回一个套接字描述符用于后续的通信操作。
- 参数说明：
  - `domain`：一个整数值，用于指定套接字的协议族，常用的有 `AF_INET`（IPv4协议族）和 `AF_INET6`（IPv6协议族），`AF_UNIX or AF_LOCAL`（本地协议族）。
  - `type`：一个整数值，用于指定套接字的类型，常用的有 `SOCK_STREAM`（流式套接字，用于TCP通信）和 `SOCK_DGRAM`（数据报套接字，用于UDP通信）。
  - `protocol`：一个整数值，用于指定套接字的协议，常用的有 0（自动选择合适的协议）或 `IPPROTO_TCP`（TCP协议）和 `IPPROTO_UDP`（UDP协议）。
- 返回值：如果套接字创建成功，函数返回一个非负整数值作为套接字描述符。如果失败，函数返回 -1，并设置全局变量 `errno` 表示具体的错误信息。

**2. `bind` 函数：**

```c
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

- 功能说明：`bind()` 函数用于将一个本地地址绑定到套接字。
- 参数说明：
  - `sockfd`：一个整数值，表示要绑定的套接字描述符。
  - `addr`：一个指向 `struct sockaddr` 结构的指针，表示要绑定的本地地址。需要根据套接字的协议族指定正确类型的地址结构，常用的是 `struct sockaddr_in`（IPv4地址结构）。
  - `addrlen`：一个整数值，表示本地地址结构的长度。
- 返回值：如果绑定成功，函数返回 0。如果失败，函数返回 -1，并设置全局变量 `errno` 表示具体的错误信息。

**3. `listen` 函数：**

```c
#include <sys/types.h>
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```

- 功能说明：`listen()` 函数用于将套接字设置为监听状态，准备接收客户端的连接请求。
- 参数说明：
  - `sockfd`：一个整数值，表示要监听的套接字描述符。
  - `backlog`：一个整数值，表示监听队列的最大长度，即同时等待处理的连接请求数量。
- 返回值：如果监听成功，函数返回 0。如果失败，函数返回 -1，并设置全局变量 `errno` 表示具体的错误信息。

**4. `accept` 函数：**

```c
#include <sys/types.h>
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

- 功能说明：`accept()` 函数用于接收客户端的连接请求，并创建一个新的套接字用于与客户端进行通信。(`阻塞`)
- 参数说明：
  - `sockfd`：一个整数值，表示监听套接字的描述符，即由 `listen()` 创建的套接字。
  - `addr`：一个指向 `struct sockaddr` 结构的指针，用于存储连接客户端的地址。
  - `addrlen`：一个指向整数的指针，表示 `addr` 结构的长度。在调用函数前，需要将其设置为 `struct sockaddr` 结构的长度。
- 返回值：如果连接成功，函数返回一个新的套接字描述符，用于与客户端进行通信。如果失败，函数返回 -1，并设置全局变量 `errno` 表示具体的错误信息。

**5. `connect` 函数：**

```c
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

- 功能说明：`connect()` 函数用于客户端向服务器发起连接请求。(阻塞)
- 参数说明：
  - `sockfd`：一个整数值，表示客户端套接字的描述符。
  - `addr`：一个指向 `struct sockaddr` 结构的指针，表示服务器的地址。需要根据套接字的协议族指定正确类型的地址结构，常用的是 `struct sockaddr_in`（IPv4地址结构）。
  - `addrlen`：一个整数值，表示服务器地址结构的长度。
- 返回值：如果连接成功，函数返回 0。如果失败，函数返回 -1，并设置全局变量 `errno` 表示具体的错误信息。

# **4.** **`shutdown`**

**1. 功能说明：**

`shutdown` 函数用于关闭一个已连接的套接字的发送和/或接收功能。它允许程序在通信结束后逐步关闭套接字，通常在客户端或服务器完成通信后使用。

**2. 函数原型：**

```c
#include <sys/socket.h>

int shutdown(int sockfd, int how);
```

**3. 参数说明：**

- `sockfd`：一个整数值，表示要关闭的套接字描述符。
- `how`：一个整数值，表示关闭的方式。可以是以下常量之一：
  - `SHUT_RD`：关闭接收功能。在调用后，套接字将不能再接收数据。
  - `SHUT_WR`：关闭发送功能。在调用后，套接字将不能再发送数据。
  - `SHUT_RDWR`：关闭接收和发送功能。在调用后，套接字将完全关闭，不能再接收或发送数据。

**4. 返回值：**

- 如果调用成功，函数返回 0。
- 如果调用失败，返回值为 -1，并设置全局变量 `errno` 表示具体的错误信息。

**5. 总结：**

`shutdown` 函数用于在通信结束后逐步关闭套接字的发送和/或接收功能。通常在客户端或服务器完成通信后，调用 `shutdown` 函数可以告诉对方不再发送或接收数据。这对于优雅地关闭连接很有用，因为它允许在不立即关闭套接字的情况下完成尚未完成的数据传输。

示例用法：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, 10) == -1) {
        perror("listen");
        close(sockfd);
        return 1;
    }

    int client_sockfd = accept(sockfd, NULL, NULL);
    if (client_sockfd == -1) {
        perror("accept");
        close(sockfd);
        return 1;
    }

    // 在通信结束后逐步关闭套接字
    shutdown(client_sockfd, SHUT_RDWR);
    close(client_sockfd);
    close(sockfd);

    return 0;
}
```

在上述示例中，服务器在完成通信后使用 `shutdown` 函数关闭了客户端的套接字，然后使用 `close` 函数关闭了套接字描述符。这样可以确保连接得到适当地关闭，不再接收或发送数据。

`shutdown` 函数对于在通信结束后逐步关闭套接字非常有用，以保证连接的优雅关闭。


# 5. IO多路复用

## 5.1 select()

**1. 功能说明：**

`select` 函数用于监视一组文件描述符，等待其中的一个或多个文件描述符就绪（可读、可写或出现异常），或者等待指定的时间超时。它通常用于实现非阻塞的I/O操作和多路复用，以提高程序的效率和响应性。

**2. 函数原型：**

```c
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```

**3. 参数说明：**

- `nfds`：一个整数值，表示要监视的最大文件描述符值加一。
- `readfds`：一个指向 `fd_set` 结构的指针，表示要监视可读操作的文件描述符集合。
- `writefds`：一个指向 `fd_set` 结构的指针，表示要监视可写操作的文件描述符集合。
- `exceptfds`：一个指向 `fd_set` 结构的指针，表示要监视异常情况的文件描述符集合。
- `timeout`：一个指向 `struct timeval` 结构的指针，表示超时时间，如果设置为 `NULL`，则表示无限等待。

**4. 返回值：**

- 如果有文件描述符就绪或超时，函数返回就绪文件描述符的总数。
- 如果出错，返回值为 -1，并设置全局变量 `errno` 表示具体的错误信息。

**5. 总结：**

`select` 函数用于实现多路复用的I/O操作，可以同时监视多个文件描述符的可读、可写和异常情况。通过在超时内等待文件描述符就绪，程序可以有效地管理多个I/O操作而不会阻塞主线程。它被广泛用于实现网络服务器和客户端程序，以及非阻塞的输入/输出。

示例用法：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    struct timeval timeout;
    timeout.tv_sec = 5;  // 设置超时时间为 5 秒
    timeout.tv_usec = 0;

    FD_SET(sockfd, &read_fds);

    int ready_fds = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
    if (ready_fds == -1) {
        perror("select");
        close(sockfd);
        return 1;
    }

    if (ready_fds == 0) {
        printf("Timeout reached.\n");
    } else {
        if (FD_ISSET(sockfd, &read_fds)) {
            printf("Socket is ready for reading.\n");
        }
    }

    close(sockfd);

    return 0;
}
```

在上述示例中，我们使用 `select` 函数等待 `sockfd` 套接字的可读事件，设置超时时间为 5 秒。如果在超时时间内套接字可读，程序将打印消息，否则将打印超时消息。

## 5.2 poll()

**1. 功能说明：**

`poll` 函数用于监视一组文件描述符，等待其中的一个或多个文件描述符就绪（可读、可写或出现异常），或者等待指定的时间超时。与 `select` 函数类似，`poll` 也用于实现非阻塞的I/O操作和多路复用，以提高程序的效率和响应性。

**2. 函数原型：**

```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

**3. 参数说明：**

- `fds`：一个指向 `struct pollfd` 结构数组的指针，每个元素表示一个要监视的文件描述符及其关注的事件。
  ```c
  struct pollfd
  {
    int fd;    //委托内核检测的文件描述符
    short int events;  // 委托内核检测文件描述符的什么事件
    short int revents;  // 文件描述符实际发生的事件
  }
  ```
- `nfds`：一个 `nfds_t` 类型的值，表示 `fds` 数组中的文件描述符数量。
- `timeout`：一个整数值，表示超时时间（以毫秒为单位），如果设置为 `-1`，表示无限等待，如果设置为 `0`，表示立即返回。

**4. 返回值：**

- 如果有文件描述符就绪或超时，函数返回就绪文件描述符的数量。
- 如果出错，返回值为 -1，并设置全局变量 `errno` 表示具体的错误信息。

**5. 总结：**

`poll` 函数与 `select` 类似，用于实现多路复用的I/O操作，但在某些方面更灵活和高效。相对于 `select`，`poll` 支持的文件描述符数量没有上限，不需要额外维护文件描述符集合，而是使用一个数组进行管理，因此在大规模应用中更适用。此外，`poll` 可以避免一些 `select` 函数的限制。

示例用法：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>

int main() {
    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO; // 标准输入文件描述符
    fds[0].events = POLLIN;   // 监视可读事件

    int ready_fds = poll(fds, 1, 5000); // 等待 5 秒

    if (ready_fds == -1) {
        perror("poll");
        return 1;
    }

    if (ready_fds == 0) {
        printf("Timeout reached.\n");
    } else {
        if (fds[0].revents & POLLIN) {
            printf("Standard input is ready for reading.\n");
        }
    }

    return 0;
}
```

在上述示例中，我们使用 `poll` 函数等待标准输入的可读事件，设置超时时间为 5 秒。如果在超时时间内标准输入可读，程序将打印消息，否则将打印超时消息。

`poll` 函数是一个用于实现多路复用的重要函数，相对于 `select` 具有更高的灵活性和效率。它可以在一组文件描述符中等待就绪事件或超时。


## 5.3 epoll_create()


**1. 功能说明：**

`epoll_create` 函数用于创建一个新的 epoll 实例，用于实现高效的 I/O 事件多路复用。`epoll` 是 Linux 下的一种事件通知机制，能够同时监视多个文件描述符上的事件，并在事件发生时进行处理，适用于高性能的网络编程。

**2. 函数原型：**

```c
#include <sys/epoll.h>

int epoll_create(int size);
```

**3. 参数说明：**

- `size`：一个整数值，表示创建的 epoll 实例的大小估计（在一些实现中无效）。该参数在新的内核版本中已经被忽略，可以设置为任意大于0的值。

**4. 返回值：**

- 如果调用成功，`epoll_create` 函数返回一个新的 epoll 实例的文件描述符。
- 如果调用失败，返回值为 -1，并设置全局变量 `errno` 表示具体的错误信息。

**5. 总结：**

`epoll_create` 函数用于创建一个新的 epoll 实例，为之后的事件多路复用操作提供基础。通过调用 `epoll_create` 创建一个 epoll 实例，可以使用其返回的文件描述符来管理和监视需要关注的事件。`epoll` 机制在处理大量文件描述符的高并发情况下，具有较好的性能表现。

示例用法：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

int main() {
    int epoll_fd = epoll_create(1); // 创建 epoll 实例
    if (epoll_fd == -1) {
        perror("epoll_create");
        return 1;
    }

    // 其他操作...

    close(epoll_fd);

    return 0;
}
```

在上述示例中，我们使用 `epoll_create` 函数创建一个新的 epoll 实例，并将返回的文件描述符存储在 `epoll_fd` 中。然后可以使用这个文件描述符来进行事件多路复用操作。

## 5.4 epoll_ctl()

**1. 功能说明：**

`epoll_ctl` 函数用于在已创建的 epoll 实例中添加、修改或删除文件描述符的事件，以便后续使用 `epoll_wait` 函数监视这些事件的就绪状态。

**2. 函数原型：**

```c
#include <sys/epoll.h>

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```

**3. 参数说明：**

- `epfd`：一个整数值，表示要操作的 epoll 实例的文件描述符。
- `op`：一个整数值，表示要进行的操作，可以是以下值之一：
  - `EPOLL_CTL_ADD`：将文件描述符添加到 epoll 实例中，以监视事件。
  - `EPOLL_CTL_MOD`：修改文件描述符在 epoll 实例中的事件。
  - `EPOLL_CTL_DEL`：从 epoll 实例中删除文件描述符。
- `fd`：一个整数值，表示要操作的文件描述符。
- `event`：一个指向 `struct epoll_event` 结构的指针，表示要添加或修改的事件信息。
  ```c
    struct epoll_event
    {
      uint32_t events;  // epoll events
        // - EPOLLIN
        // - EPOLLOUT
        // - EPOLLERR
      epoll_data_t data;
    }
    typedef union epoll_data
    {
      void *ptr;
      int fd;
      uint32_t u32;
      uint64_t u64;
    }epoll_data_t;
  ```

**4. 返回值：**

- 如果调用成功，`epoll_ctl` 函数返回 `0`。
- 如果调用失败，返回值为 `-1`，并设置全局变量 `errno` 表示具体的错误信息。

**5. 总结：**

`epoll_ctl` 函数用于向已创建的 epoll 实例中添加、修改或删除文件描述符的事件信息，以便在后续使用 `epoll_wait` 函数监视文件描述符上的事件就绪状态。通过 `epoll_ctl` 可以动态地管理要关注的事件，从而实现高效的 I/O 事件多路复用。

示例用法：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

int main() {
    int epoll_fd = epoll_create(1); // 创建 epoll 实例
    if (epoll_fd == -1) {
        perror("epoll_create");
        return 1;
    }

    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN; // 监视可读事件
    event.data.fd = STDIN_FILENO; // 标准输入文件描述符

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        close(epoll_fd);
        return 1;
    }

    // 其他操作...

    close(epoll_fd);

    return 0;
}
```

在上述示例中，我们使用 `epoll_ctl` 函数将标准输入文件描述符添加到 epoll 实例中，以监视可读事件。然后可以使用 `epoll_wait` 函数等待标准输入的可读事件就绪。

## 5.5 epoll_wait()

**1. 功能说明：**

`epoll_wait` 函数用于等待已注册的文件描述符上的事件就绪，并将就绪的事件信息返回给调用者。它是 Linux 下使用 epoll 机制进行事件多路复用的核心函数之一。

**2. 函数原型：**

```c
#include <sys/epoll.h>

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

**3. 参数说明：**

- `epfd`：一个整数值，表示要等待的 epoll 实例的文件描述符。
- `events`：一个指向 `struct epoll_event` 结构数组的指针，用于存储就绪的事件信息。
- `maxevents`：一个整数值，表示 `events` 数组的大小，表示最多可以存储多少个事件信息。
- `timeout`：一个整数值，表示等待事件的超时时间（以毫秒为单位）。如果设置为 `-1`，表示无限等待；如果设置为 `0`，表示立即返回，如果设置为大于 `0`，表示等待指定的毫秒数。

**4. 返回值：**

- 如果有事件就绪，`epoll_wait` 函数返回就绪事件的数量。
- 如果在超时时间内没有事件就绪，返回值为 `0`。
- 如果调用出错，返回值为 `-1`，并设置全局变量 `errno` 表示具体的错误信息。

**5. 总结：**

`epoll_wait` 函数是 `epoll` 机制的核心之一，用于等待已注册的文件描述符上的事件就绪。它可以同时等待多个文件描述符的事件，一旦有事件就绪，就将相应的事件信息存储在给定的数组中返回。

示例用法：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

int main() {
    int epoll_fd = epoll_create(1); // 创建 epoll 实例
    if (epoll_fd == -1) {
        perror("epoll_create");
        return 1;
    }

    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN; // 监视可读事件
    event.data.fd = STDIN_FILENO; // 标准输入文件描述符

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        close(epoll_fd);
        return 1;
    }

    struct epoll_event events[10];
    int ready_events = epoll_wait(epoll_fd, events, 10, -1); // 无限等待
    if (ready_events == -1) {
        perror("epoll_wait");
        close(epoll_fd);
        return 1;
    }

    for (int i = 0; i < ready_events; ++i) {
        if (events[i].data.fd == STDIN_FILENO) {
            printf("Standard input is ready for reading.\n");
        }
    }

    close(epoll_fd);

    return 0;
}
```

# 6. UDP读写

## 6.1 sendto()

**1. 功能说明：**

`sendto` 函数用于向指定的目标地址发送数据，适用于支持数据报式（无连接）套接字（如UDP套接字）。它可以在网络编程中用于向远程主机发送数据。

**2. 函数原型：**

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
```

**3. 参数说明：**

- `sockfd`：一个整数值，表示要发送数据的套接字描述符。
- `buf`：一个指向存储要发送数据的缓冲区的指针。
- `len`：一个 `size_t` 类型的值，表示要发送的数据的大小（字节数）。
- `flags`：一个整数值，表示发送操作的附加选项，常用的有：
  - `0`：默认选项，无附加操作。
  - `MSG_DONTWAIT`：非阻塞发送，如果缓冲区已满则立即返回。
  - `MSG_CONFIRM`：确认数据发送，仅适用于某些协议。
  - 等等，具体选项取决于操作系统和协议。
- `dest_addr`：一个指向目标地址结构的指针，表示数据发送的目标地址。
- `addrlen`：一个 `socklen_t` 类型的值，表示目标地址结构的大小。

**4. 返回值：**

- 如果调用成功，`sendto` 函数返回实际发送的字节数。
- 如果调用失败，返回值为 `-1`，并设置全局变量 `errno` 表示具体的错误信息。

**5. 总结：**

`sendto` 函数用于在无连接的数据报式套接字中向指定的目标地址发送数据。通过提供套接字描述符、数据缓冲区、目标地址和其他参数，可以将数据发送到远程主机。它适用于在网络编程中使用UDP协议进行数据传输。



## 6.2 recvfrom()

**1. 功能说明：**

`recvfrom` 函数用于从数据报式（无连接）套接字（如UDP套接字）接收数据，并获取数据发送方的地址信息。它适用于网络编程中接收远程主机发送的数据。

**2. 函数原型：**

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
```

**3. 参数说明：**

- `sockfd`：一个整数值，表示要接收数据的套接字描述符。
- `buf`：一个指向存储接收数据的缓冲区的指针。
- `len`：一个 `size_t` 类型的值，表示缓冲区的大小，即要接收的数据的最大字节数。
- `flags`：一个整数值，表示接收操作的附加选项，常用的有：
  - `0`：默认选项，无附加操作。
  - `MSG_WAITALL`：等待接收所有请求的数据，直到缓冲区满或连接关闭。
  - `MSG_PEEK`：从缓冲区查看数据，但不将其从缓冲区中移除。
  - 等等，具体选项取决于操作系统和协议。
- `src_addr`：一个指向存储发送方地址信息的结构的指针。当调用成功时，该结构将被填充以标识发送方的地址信息。
- `addrlen`：一个指向 `socklen_t` 类型的指针，用于指示 `src_addr` 结构的大小。

**4. 返回值：**

- 如果调用成功，`recvfrom` 函数返回实际接收的字节数。
- 如果连接关闭，返回值为 `0`。
- 如果调用失败，返回值为 `-1`，并设置全局变量 `errno` 表示具体的错误信息。

**5. 总结：**

`recvfrom` 函数用于从无连接的数据报式套接字中接收数据，并获取发送方的地址信息。通过提供套接字描述符、数据缓冲区、接收选项和地址结构，可以从远程主机接收数据。它适用于在网络编程中使用UDP协议进行数据传输。





# <center> Q&A

## 1. **select()原理？优缺点？**

- 缺点
  - 每次调用`select`，都需要把`fd_set`从用户态拷贝到内核态，在文件描述符较多的时候开销较大
  - 每次调用`select`需要在内核便利传进来的`fd_set`，在文件描述符较多的时候开销较大
  - `select`支持的文件描述符太少了，默认为1024
  - `fd_set`不能重用

## 2. **`listen(int sockfd, int backlog)`做了哪些事情？**

- 将套接字`sockfd`设置为监听状态`TCP_LISTEN`
- 创建半连接队列(哈希表实现)和全连接队列(链表实现)，通过`backlog`指定队列的大小，自`linux 2.2`开始这个参数只指定全连接队列的大小，半连接队列大小由`/proc/sys/net/ipv4/tcp_max_syn_backlog`指定
- 以上操作均由内核完成，`listen`仅仅把`sockfd`和`backlog`告诉内核，同通知内核完成这些操作。

3. **客户端调用`connect()`的时候做了哪些事情？**

- 把本地`socket`状态设置成`TCP_SYN_SENT`
- 自动一个可用的端口
- 阻塞等待，直到内核完成与服务器端的握手
- 以上操作均由内核完成

4. **TCP三次握手是怎么完成的？**

- 客户端调用`connect()`，发送`SYN`请求，并设置重传定时器
- 请求到达服务端，通过软中断处理函数处理请求，首先判断半连接队列是否已满，满了的话判断是否开启`SYNCOOKIE`，没有开启则会被丢弃，否则服务端给客户端发送`SYNACK`，申请连接对象保存添加到半连接队列中，并启动重传定时器
- 客户端相应`SYNACK`，发送`ACK`，清除原来的重传定时器，并设置新的定时器。`socket`状态更改为`ESTABLISHED`
- 服务端相应客户端`ACK`，把半连接对象删除，创建全连接对象加入全连接队列，并且状态设置成`ESTABLISHED`
- 最后服务端通过`accept`阻塞式的从全连接队列中取出`socket`，建立`socket`缓冲区(每个客户端都有一个单独的缓冲区)




