#ifndef E_S_EPOLLPOLLER_H
#define E_S_EPOLLPOLLER_H

#include <vector>
#include <sys/epoll.h>

#include "Poller.h"
#include "TimeStamp.h"

class Channel;

// The struct epoll_event is defined as:

// typedef union epoll_data {
//     void    *ptr;
//     int      fd;
//     uint32_t u32;
//     uint64_t u64;
// } epoll_data_t;

// struct epoll_event {
//     uint32_t     events;    /* Epoll events */
//     epoll_data_t data;      /* User data variable */
// };


class EpollPoller : public Poller
{
public:
    EpollPoller(EventLoop* loop);
    ~EpollPoller() override;

    TimeStamp poll(int timeout, ChannelList* channels) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;

private:
    using EventList = std::vector<epoll_event>;
    static const int initReturnEventSize = 16;

    void activeChannelsList(int nums, ChannelList* actviechannels) const;
    // give back the happened events from epoll to channel
    void update(int operation, Channel* channel);

    int epollfd_;
    EventList events_;
};



#endif
