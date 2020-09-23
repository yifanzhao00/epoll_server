#include "Poller.h"
#include "EpollPoller.h"

// in this project, we only consider epoll
Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    return new EpollPoller(loop);
}