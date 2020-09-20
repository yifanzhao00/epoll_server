#include "Poller.h"

// in this project, we only consider epoll
Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    return nullptr;
}