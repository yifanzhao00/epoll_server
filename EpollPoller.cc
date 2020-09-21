#include "EpollPoller.h"
#include "Logger.h"
#include "Channel.h"

#include <errno.h>
#include <unistd.h> // close()
#include <string.h>


// status channel in the epoll
// assign to the index in the channel
const int kNew = -1;
const int kAdd = 1;
const int kDelete = 2;

// epoll_create
EpollPoller::EpollPoller(EventLoop* loop)
            : Poller(loop)
            , epollfd_(::epoll_create1(EPOLL_CLOEXEC))
            , events_(initReturnEventSize)
{
    if (epollfd_ < 0) {
        LOG_FATAL("epoll_create error:%d \n", errno);
    }
}

// close epoll
EpollPoller::~EpollPoller()
{
    ::close(epollfd_);
}

// epoll_wait
TimeStamp EpollPoller::poll(int timeout, ChannelList* channels)
{
    // LOG_DEBUG
    LOG_INFO("func=%s => fd total count:%lu \n", __FUNCTION__, channels_.size());
    int nums = ::epoll_wait(epollfd_, &(*events_.begin()), static_cast<int>(events_.size()), timeout);
    int saveErrno = errno;
    TimeStamp now(TimeStamp::now());

    if (nums > 0) {
        LOG_INFO("%d events happened \n", nums);
        activeChannelsList(nums, channels);
        if (nums == events_.size()) {
            events_.resize(events_.size() * 2);
        }
    }else if (nums == 0) {
        LOG_DEBUG("%s, timeout! \n", __FUNCTION__);
    }else {
        if (errno != saveErrno) {
            errno = saveErrno;
            LOG_ERROR("EpollPoller::poll() error!");
        }
    }

    return now;
}    

//epoll_ctl
void EpollPoller::update(int operation, Channel* channel)
{
    epoll_event event;
    bzero(&event, sizeof event);

    int fd = channel->fd();
    event.events = channel->events();
    event.data.fd = fd;
    event.data.ptr = channel;

    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
        if (operation == EPOLL_CTL_DEL) {
            LOG_ERROR("epoll_ctl del error:%d\n", errno);
        }else {
            LOG_FATAL("epoll_ctl add/mod error:%d\n", errno);
        }
    }
}

void EpollPoller::updateChannel(Channel* channel) {
    const int index = channel->index();
    LOG_INFO("func=%s => fd=%d events=%d index=%d \n", __FUNCTION__, channel->fd(), channel->events(), index);

    if (index == kNew || index == kDelete) {
        if (index == kNew) {
            channels_.insert({channel->fd(), channel});
        }
        channel->set_index(kAdd);
        update(EPOLL_CTL_ADD, channel);
    }else {
        if (channel->isNoneEvent()) {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDelete);
        }else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EpollPoller::removeChannel(Channel* channel) {
    int fd = channel->fd();
    channels_.erase(fd);
    LOG_INFO("func=%s => fd=%d\n", __FUNCTION__, fd);

    int index = channel->index();
    if (index == kAdd) {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(kNew);
}




void EpollPoller::activeChannelsList(int nums, ChannelList* actviechannels) const
{
    for (int i = 0; i < nums; ++i) {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        actviechannels->push_back(channel);
    }
}