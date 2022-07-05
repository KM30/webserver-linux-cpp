#ifndef HTTPCONNECTTION_H
#define HTTPCONNECTTION_H

#include <netinet/in.h>

class http_conn
{
public:
    http_conn(){}
    ~http_conn(){}
public: 
    void init(int sockfd,const struct sockaddr_in& addr);// 初始化新接受的连接
    void close_conn();// 关闭连接
    bool read();// 非阻塞读
    bool write();// 非阻塞写
    void process();// 处理客户端请求  
private:
    //void init();// 初始化连接


public:
    static int m_epollfd;// 所有socket上的事件都被注册到同一个epoll内核事件中，所以设置成静态的
    static int m_user_count;// 统计用户的数量
private:
    int m_socketfd;// 该HTTP连接的socket文件描述符
    sockaddr_in m_address;//和对方的socket地址
};

#endif