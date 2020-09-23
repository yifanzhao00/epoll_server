#ifndef E_S_EVENTLOOP
#define E_S_EVENTLOOP

#include <functional>
#include <vector>
#include <atomic>
#include <memory>
#include <mutex>

#include "noncopyable.h"
#include "TimeStamp.h"
#include "CurrentThread.h"

class Channel;
class Poller;


class EventLoop : noncopyable
{
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    // begin wake and end the loop
    void loop();
    void quit();
    void wakeup();

    // method the eventloop to connect between poller and channel
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

    //callback method
    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);

    //get and set method
    TimeStamp pollReturnTime() const { return pollReturnTime_; }
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
private:
    using ChannelList = std::vector<Channel*>;
    
    // flags about loop or quit
    std::atomic_bool looping_;
    std::atomic_bool quit_;

    // thread_local_id
    const pid_t threadId_;

    //return time from epoll
    TimeStamp pollReturnTime_;

    //wakeup channel used to wake up the loop
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;

    // epoll the eventloop has
    std::unique_ptr<Poller> poller_;

    // return to channel to update
    ChannelList activeChannels_;

    // callback function flag and storage
    std::atomic_bool callingPendingFunctors_;
    std::vector<Functor> pendingFunctors_;
    std::mutex mutex_;

    void handleRead(); // wake up channel register
    void doPendingFunctors(); // execute callback
};



#endif