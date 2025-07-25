#include "include/Epoll.h"
#include "include/Channel.h"
#include "include/util.h"
#include <string.h>
#include <unistd.h>
#define MAX_EVENTS 1000

Epoll::Epoll()
{
  epfd_ = epoll_create1(0);
  ErrorIf(epfd_ == -1, "epoll create error");
  events_ = new epoll_event[MAX_EVENTS];
  memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Epoll::~Epoll()
{
  if (epfd_ != -1)
  {
    close(epfd_);
    epfd_ = -1;
  }
  delete[] events_;
}

std::vector<Channel *> Epoll::Poll(int timeout)
{
  std::vector<Channel *> active_channels;
  int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
  ErrorIf(nfds == -1, "epoll wait error");
  for (int i = 0; i < nfds; ++i)
  {
    Channel *ch = (Channel *)events_[i].data.ptr;
    ch->SetReadyEvents(events_[i].events);
    active_channels.push_back(ch);
  }
  return active_channels;
}

void Epoll::UpdateChannel(Channel *ch)
{
  int fd = ch->GetFd();
  struct epoll_event ev{};
  ev.data.ptr = ch;
  ev.events = ch->GetListenEvents();
  if (!ch->GetInEpoll())
  {
    ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    ch->SetInEpoll();
  }
  else
  {
    ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
  }
}

void Epoll::DeleteChannel(Channel *ch)
{
  int fd = ch->GetFd();
  ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll delete error");
  ch->SetInEpoll(false);
}
