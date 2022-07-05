#ifndef HTTPCONNECTTION_H
#define HTTPCONNECTTION_H

#include <netinet/in.h>

class http_conn
{
/* 1-成员函数 */
public:
    http_conn(){}
    ~http_conn(){}
    void init(int sockfd,const struct sockaddr_in& addr);   // 初始化新接受的连接
    void close_conn();                                      // 关闭连接
    bool read();                                            // 非阻塞读
    bool write();                                           // 非阻塞写
    void process();                                         // 处理HTTP请求的入口函数 


/* 2-成员变量 */
public:
    static int m_epollfd;                       // 所有socket上的事件都被注册到同一个epoll内核事件中，所以设置成静态的
    static int m_user_count;                    // 统计用户的数量
    // http：解析请求报文和撰写响应报文
    static const int READ_BUFFER_SIZE = 2048;   // 读缓冲区的大小
private:
    int m_socketfd;                             // 该HTTP连接的socket文件描述符
    sockaddr_in m_address;                      // 和对方的socket地址
    // http：解析请求报文和撰写响应报文
    char m_read_buf[ READ_BUFFER_SIZE ];        // 读缓冲区
    int m_read_index = 0;                           // 标识读缓冲区中已经读入的客户端数据的最后一个字节的下一个位置
};

#endif