#ifndef E_S_CHANNEL_H
#define E_S_CHANNEL_H

#include "noncopyable.h"
#include "TimeStamp.h"

#include <functional>
#include <memory>

class EventLoop;

/**
 * channel encapsulate the sockfd and the interestd events 
 * including EPOLLIN and EPOLLOUT, it also bind the events which poller returns.
*/
class Channel: noncopyable
{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(TimeStamp)>;

    Channel(EventLoop* loop, int fa);
    ~Channel();

    //callback function
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    // get and set Method
    int fd() const { return fd_; }
    int events() const { return events_; }
    int index() const { return index_ ;}
    EventLoop* ownerloop() { return loop_; }
    void set_index(int index) { index_ = index; }
    void set_revents(int revents) { revents_ = revents; }

    //return event status
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }

    //set events in the epoll 
    void enableReading();
    void disableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();

    // remove the channel from the eventloop and epoll
    void remove();

    // channel cannot callback if it is removed
    void tie(const std::shared_ptr<void>& ptr);

    void handleEvent(TimeStamp receiveTime);

private:
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    EventLoop *loop_;
    const int fd_;
    int events_; // interested events
    int revents_; // returned events
    int index_; // status in the epoll

    // event status
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    // comcurrency issue
    std::weak_ptr<void> tie_;
    bool tied_;

    // update the events in epoll(epoll_ctl)
    void update();
    void handleEventWithGuard(TimeStamp receiveTime);
};











#endif

