#include "EventLoop.h"
#include "Logger.h"
#include "Poller.h"
#include "Channel.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <errno.h>
#include <memory>

// each thread has its own value and guarantee 
// one thread per loop
__thread EventLoop *t_loopInThisThread = nullptr;

// maxtimeout for the epoll_wait
const int kPollTimes = 10000;

int createEventfd()
{
    /**
     * int eventfd(unsigned int initval, int flags);
     * eventfd()  creates  an  "eventfd  object"  that can be used as an event
       wait/notify mechanism by user-space applications, and by the kernel  to
       notify  user-space  applications  of  events.
    */

    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0) {
        LOG_FATAL("eventfd error:%d\n", errno);
    }
    return evtfd;
}

EventLoop::EventLoop()
          : looping_(false)
          , quit_(false)
          , callingPendingFunctors_(false)
          , threadId_(CurrentThread::tid())
          , poller_(Poller::newDefaultPoller(this))
          , wakeupFd_(createEventfd())
          , wakeupChannel_(new Channel(this, wakeupFd_))
{
    LOG_DEBUG("eventloop created %p in thread %d \n", this, threadId_);
    
    if (t_loopInThisThread) {
        LOG_FATAL("Another eventloop %p exits in this thread %d \n", t_loopInThisThread, threadId_);
    }else {
        t_loopInThisThread = this;
    }

    // set wakeup channel in order to wake up the specific loop 
    // by enable its read event
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::loop()
{
    looping_ = true;
    quit_ = false;

    LOG_INFO("eventloop %p start looping \n", this);

    while(!quit_) {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimes, &activeChannels_);
        for (Channel* channel : activeChannels_) {
            channel->handleEvent(pollReturnTime_);
        }
        // perform the callback function the loop needed to 
        doPendingFunctors();
    }

    LOG_INFO("eventloop %p stop looping \n", this);
    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;

    // if it not quit the current thread, wakeup the current thread
    if (!isInLoopThread()) {
        wakeup();
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1; // 8 byte
    ssize_t n = write(wakeupFd_, &one, sizeof one);
    if (n != sizeof one) {
        LOG_ERROR("eventloop::wakeup() writes %lu bytes instead of 8 \n", n);
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel)
{
    return poller_->hasChannel(channel);
}

void EventLoop::runInLoop(Functor cb)
{
    if (isInLoopThread()) {
        cb();
    }else {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(Functor cb)
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pendingFunctors_.emplace_back(cb);
    }

    // wake up the current thread or the current thread
    // has new coming callback functions
    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof one);

    if (n != sizeof one) {
        LOG_ERROR("eventloop::handleRead() reads %lu bytes instead of 8", n);
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        std::unique_lock<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for (const Functor &functor : functors) {
        functor();
    }

    callingPendingFunctors_ = false;
}
