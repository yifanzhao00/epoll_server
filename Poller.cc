#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop* loop)
        : loop_(loop)
{}

bool Poller::hasChannel(Channel* channel) const
{
    int fd_tmp = channel->fd();
    return channels_.count(fd_tmp) && (channels_.at(fd_tmp) == channel);
}