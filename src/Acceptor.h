#pragma once
#include <functional>
class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *acceptChannel;
    std::function<void(Socket *)> newConnectionCallback;

public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket *)>);
};