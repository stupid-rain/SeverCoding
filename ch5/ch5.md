# socket网络学习

## socket

### 5.13专用socker地址

- UNIX本地域协议族使用如下专用socket地址结构体

```cpp
    #include<sys/un.h>
    struct sockaddr_un
    {
        sa_family_t sin_family; /* 地址族:AF_UNIX*/
        char sun_path[108]; /*文件路径名*/ 
    }
```

- TCP/IP协议族有sockaddr_in 和sockaddr_in6 两个专用socket地址结构体

```cpp

struct sockaddr_in
{
    da_family_t sin_family; /*地址族:AF_INET*/
    u_int16_t sin_port; /*端口号，要用网络字节序表示*/
    struct in_addr sin_addr;/* IPv4地址结构体，见下面*/
};

struct in_addr
{
    u_int32_t s_addr; /*IPv4地址，要用网络字节序表示*/
};

struct sockaddr_in6
{
    sa_family_t sin6_family;/*地址族:AF_INET6*/
    u_int16_t sin6_port;/*端口号，要用网络字节序表示*/
    u_int32_t sin6_flowinfo; /*流信息，应设置为0*/
    struct in6_addr sin6_addr;/* IPv6地址结构体，见下面*/
    u_int32_t sin6_scope_id;/*scope ID,尚处于试验阶段*/
};
struct in6_addr
{
    unsigned char sa_addr[16];/*IPv6地址，要用网络字节序表示*/
```

## 5.2创建socket

Unix/Linux ：所有东西都是文件。socket也是，它是可读、可写、可控制、可关闭的文件描述符。

``` cpp
#include<sys/types.h>
#include<sys/socket.h>
int socket(int domain,int type, int protocol);
/*
domain 是使用那个底层协议族，对TCP/IP协议族来说，PF_INET(IPv4)、PF_INET6(IPv6)
对于UNIX本地域协议族而言，该参数应该设置为PF_UNIX。关于socket系统调用支持的所有协议族，
type参数指定服务类型。服务类型主要有SOCK_STREAM服务（流服务）和SOCK_UGRAM（数据报）服务。对TCP/IP协议族而言，其值取SOCK_STREAM表示传输层使用TCP协议，取SOCK_DGRAM表示传输层使用UDP协议
*/
```

## 5.3命名socket

服务器程序中，要命名socket来告知client 如何连接，client不需要命名socket
命名socket的系统调用是bind

```cpp
#include<sys/types.h>
#include<sys/socket.h>
int bind(int sockfd,const stuct sockaddr &myaddr, socklen_t addrlen);
/*

*/

```

bind将my_addr所指的socket地址分配给未命名的sockfd文件描述符，addrlen参数指出该socket地址的长度。bind成功时返回0，失败则返回-1并设置errno。其中两种常见的errno是EACCES和EADDRINUSE，它们的含义分别是：

1. EACCES，被绑定的地址是受保护的地址，仅超级用户能够访问。比如普通用户将socket绑定到知名服务端口（端口号为0～1023）上时，bind将返回EACCES错误。
2. EADDRINUSE，被绑定的地址正在使用中。比如将socket绑定到一个处于TIME_WAIT状态的socket地址

## 5.4监听socket

socket需要通过系统调用创建一个监听队列来存放待处理的客户连接

```cpp
#include<sys/socket.h>
int listen(int sockft,int backlog);

```

## 5.5接收连接

从listen监听队列中接受一个连接

```cpp
#include<sys/types.h>
#include<sys/socket.h>
int accept(int sockfd ,struct sockaddr* addr ,socklen_t* addrlen);
```

sockfd参数是执行过Listen系统调用的监听socket
addr获取被接受连接的远端socket地址，长度有addrlen给出
返回一个新的连接socket，唯一地标识了被接受的这个连接，服务器通过读写socket来与接受连接对应的client通信

## 5.6 发起连接

client通过系统调用主动与服务器建立连接

```cpp
#include<sys/types.h>
#include<sys/socket.h>
int connect(int sockfd,const struct sockaddr*serv_addr,socklen_taddrlen);

```

## 5.7  关闭连接

- 关闭一个连接实际上就是关闭该连接对应的socket

```cpp
#include<unistd.h>
int close(int fd);
```

fd参数是待关闭的socket。不过，close系统调用并非总是立即关闭一个连接，而是将fd的引用计数减1。只有当fd的引用计数为0时，才真正关闭连接。多进程程序中，一次fork系统调用默认将使父进程中打开的socket的引用计数加1，因此我们必须在父进程和子进程中都对该socket执行close调用才能将连接关闭

- 立即终止连接

```cpp
#include<sys/socket.h>
int shutdown(int sockfd,int howto);
```

## 5.8 数据读写

### 5.8.1 TCP数据读写

用于TCP流读写的系统调用

```cpp
#include<sys/types.h>
#include<sys/socket.h>
ssize_t recv(int sockfd,void*buf,size_t len,int flags);
ssize_t send(int sockfd,const void*buf,size_t len,int flags);

```
