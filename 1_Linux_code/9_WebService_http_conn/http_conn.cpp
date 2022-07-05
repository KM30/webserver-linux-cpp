#include "http_conn.h"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>



/* 成员函数定义 */

// 1-向epoll中添加移除修改需要监听的文件描述符
int setnonblocking(int fd){
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd,F_SETFL,new_option);
    return old_option;
}
void addfd(int epollfd,int fd,bool one_shot){
    // 封装要检测的文件描述符fd->event
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP;
    if(one_shot){
        // 防止同一个通信fd被不同的线程处理
        event.events |= EPOLLONESHOT;
    }
    // 对epoll实例进行管理：添加文件描述符信息
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    // 设置文件描述符非阻塞
    setnonblocking(fd);
}
void removefd(int epollfd, int fd){
    // 从epoll中移除监听的文件描述符
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}
void modfd(int epollfd,int fd,int ev){
    // 修改文件描述符，重置socket上的EPOLLONESHOT事件，以确保下一次可读时，EPOLLIN事件能被触发
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}

// 2-初始化连接,外部调用初始化套接字地址，内部调用初始化内参
void http_conn::init(int sockfd, const struct sockaddr_in& addr){
    m_socketfd = sockfd;
    m_address = addr;
    // 端口复用
    int reuse = 1;
    setsockopt(m_socketfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    addfd(m_epollfd,sockfd,true);
    m_user_count++;
    //init();
}

// 3-关闭连接
void http_conn::close_conn(){
    if(m_socketfd != -1) {
        removefd(m_epollfd, m_socketfd);
        m_socketfd = -1;
        // 关闭一个连接，将客户总数量-1
        m_user_count--;
    }
}

// 4-循环读取客户数据，直到无数据可读或者对方关闭连接
bool http_conn::read(){
    printf("一次性读完数据\n");
    return true;
}

// 5-写HTTP响应
bool http_conn::write(){
    printf("一次性写完数据\n");
    return true;
}

// 6-由线程池中的工作线程调用，是处理HTTP请求的入口函数
void http_conn::process(){
    // 解析HTTP请求
    printf("parse request, create response\n");
    // 生成响应
}

/* 成员变量定义 */

// 所有socket上的事件都被注册到同一个epoll内核事件中，所以设置成静态的
int http_conn::m_epollfd = -1;
// 所有的客户数
int http_conn::m_user_count = 0;
