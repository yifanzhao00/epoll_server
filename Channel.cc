#include "Channel.h"
#include "Logger.h"
#include "EventLoop.h"

#include <sys/epoll.h>

/**
 * EPOLLIN
 * The associated file is available for read(2) operations.
 * EPOLLPRI
 * There is urgent data available for read(2) operations.
 * EPOLLOUT
 * The associated file is available for write(2) operations.
 * EPOLLERR
 * Error condition happened on the associated file descriptor.
 * EPOLLHUP
 * Hang up   happened   on   the   associated   file   descriptor.
 */

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

void Channel::enableReading()
{
    events_ |= kReadEvent;
    update();
}
void Channel::disableReading()
{
    events_ &= ~kReadEvent;
    update();
}
void Channel::enableWriting()
{
    events_ |= kReadEvent;
    update();
}
void Channel::disableWriting()
{
    events_ &= ~kWriteEvent;
    update();
}
void Channel::disableAll()
{
    events_ = kNoneEvent;
    update();
}

Channel::Channel(EventLoop* loop, int fd)
         : loop_(loop)
         , fd_(fd)
         , events_(0)
         , revents_(0)
         , index_(-1)
         , tied_(false)
{}

Channel::~Channel()
{}

void Channel::tie(const std::shared_ptr<void>& ptr)
{
    tie_ = ptr;
    tied_ = true;
}

void Channel::handleEvent(TimeStamp receiveTime)
{
    if (tied_) {
        std::shared_ptr<void> guard = tie_.lock();
        if (guard) {
            handleEventWithGuard(receiveTime);
        }
    }else{
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(TimeStamp receiveTime)
{
    LOG_INFO("channel handleEvent revents:%d\n", revents_);

    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }

    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            errorCallback_();
        }
    }

    if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        if (readCallback_)
        {
            readCallback_(receiveTime);
        }
    }

    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
}

void Channel::update()
{

}

void Channel::remove()
{

}
