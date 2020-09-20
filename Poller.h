#ifndef E_S_POLLER_H
#define E_S_POLLER_H

#include "noncopyable.h"
#include "TimeStamp.h"

#include <vector>
#include <unordered_map>

class Channel;
class EventLoop;

class Poller{
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop* loop);
    virtual ~Poller() = default;

    virtual TimeStamp poll(int timeout, ChannelList* channels) = 0;
    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;

    bool hasChannel(Channel* channel) const;

    //get default poller whether poll or epoll
    static Poller* newDefaultPoller(EventLoop* loop);
protected:
    using ChannelMap = std::unordered_map<int, Channel*>;

    ChannelMap channels_;
private:
    EventLoop* loop_;
};



#endif