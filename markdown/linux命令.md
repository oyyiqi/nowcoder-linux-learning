# 键盘控制

禁用笔记本键盘:`xinput set-prop {id} "Deviced Enabled" 0` or `xinput disable {id}`
启动笔记本键盘:`xinput set-prop {id} "Deviced Enabled" 1` or `xinput enable {id}`
查看{id}:`xinput list`   AT Raw Set 2 keyboard

# ps

1. 查看指定进程的线程：`ps -Lf {pid}`


# netstat  
> 查看网络连接、路由表、接口状态等网络相关信息的命令行工具


以下是一些常用的 `netstat` 命令选项：

- `netstat -a`：显示所有活动的网络连接和监听端口。
- `netstat -t`：显示 TCP 协议的网络连接。
- `netstat -u`：显示 UDP 协议的网络连接。
- `netstat -n`：以数字形式显示地址和端口号，不进行反向解析。
- `netstat -p`：显示与每个连接关联的进程信息。
- `netstat -r`：显示路由表信息。
- `netstat -i`：显示网络接口的状态和统计信息。
- `netstat -s`：显示各种网络协议的统计信息。
- `netstat -l`：仅显示监听状态的连接。

示例: netstat -anp | grep 8080   //查找8080端口的连接


### 删除当前目录下所有不是.cpp的文件
find . -type f ! -name "*.cpp" -delete



# ulimit

`ulimit` 是一个用于在Unix-like 操作系统中设置和显示用户进程资源限制的命令。它可以用来限制各种资源，例如进程数、内存使用、文件打开数等等。`ulimit` 命令通常用于终端或脚本中，以便控制特定用户的进程使用的资源。

以下是一些常用的 `ulimit` 命令选项及其含义：

1. `ulimit -a`：显示当前用户的所有资源限制信息。

2. `ulimit -c <limit>`：设置核心转储文件的大小限制，以字节为单位。当程序崩溃时，会生成一个核心转储文件，其中包含了程序崩溃时的内存和寄存器状态。

3. `ulimit -f <limit>`：设置文件大小限制，以字节为单位。这个限制控制了用户创建的文件的最大大小。

4. `ulimit -n <limit>`：设置用户可以同时打开的文件描述符的数量限制。文件描述符是用于访问文件和其他I/O资源的操作系统资源。

5. `ulimit -m <limit>`：设置内存使用限制，以 KB 为单位。这个限制控制了用户进程可以使用的最大内存量。

6. `ulimit -u <limit>`：设置用户可以创建的最大进程数限制。这个限制适用于用户的所有进程总数。

7. `ulimit -v <limit>`：设置虚拟内存限制，以 KB 为单位。虚拟内存是系统内存和交换空间的总和。

8. `ulimit -x <limit>`：设置文件锁定限制，以 KB 为单位。这个限制控制了用户可以锁定在内存中的文件数据的大小。

9. `ulimit -l <limit>`：设置可锁定内存限制，以 KB 为单位。锁定内存是一种防止内存分页到交换空间的机制，通常用于某些关键应用程序的内存保留。

请注意，`ulimit` 命令设置的资源限制通常仅适用于当前终端会话或运行的脚本，不会影响其他用户或进程。如果需要在永久基础上更改用户的资源限制，可能需要修改系统的配置文件，如 `/etc/security/limits.conf`（在一些Linux系统中）。具体操作可能因操作系统而异。