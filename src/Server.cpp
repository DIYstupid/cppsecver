#include "include/Server.h"
#include "include/Acceptor.h"
#include "include/Connection.h"
#include "include/EventLoop.h"
#include "include/Socket.h"
#include "include/ThreadPool.h"
#include "include/util.h"
#include <functional>
#include <unistd.h>

Server::Server(EventLoop *loop) : main_reactor_(loop), acceptor_(nullptr), thread_pool_(nullptr)
{
  acceptor_ = new Acceptor(main_reactor_);
  std::function<void(Socket *)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
  acceptor_->SetNewConnectionCallback(cb);

  int size = static_cast<int>(std::thread::hardware_concurrency());
  thread_pool_ = new ThreadPool(size);
  for (int i = 0; i < size; ++i)
  {
    sub_reactors_.push_back(new EventLoop());
  }

  for (int i = 0; i < size; ++i)
  {
    std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_reactors_[i]);
    thread_pool_->Add(std::move(sub_loop));
  }
}

Server::~Server()
{
  delete acceptor_;
  delete thread_pool_;
}

void Server::NewConnection(Socket *sock)
{
  ErrorIf(sock->GetFd() == -1, "new connection error");
  uint64_t random = sock->GetFd() % sub_reactors_.size();
  Connection *conn = new Connection(sub_reactors_[random], sock);
  std::function<void(int)> cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
  conn->SetDeleteConnectionCallback(cb);
  connections_[sock->GetFd()] = conn;
}

void Server::DeleteConnection(int sockfd)
{
  auto it = connections_.find(sockfd);
  if (it != connections_.end())
  {
    Connection *conn = connections_[sockfd];
    connections_.erase(sockfd);
    delete conn;
  }
}