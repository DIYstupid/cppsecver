#pragma once

#include <map>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *loop;    // 事件循环
    Acceptor *acceptor; // 用于接受TCP连接
    std::map<int, Connection *> connections;

public:
    Server(EventLoop *);
    ~Server();
    void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
};
