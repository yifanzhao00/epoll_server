#ifndef E_S_EVENTLOOPTHREADPOOL_H
#define E_S_EVENTLOOPTHREADPOOL_H

#include "noncopyable.h"

#include <functional>
#include <string>
#include <vector>
#include <memory>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    EventLoopThreadPool(EventLoop* baseloop, const std::string& name);
    ~EventLoopThreadPool();

    // round rabin method to distribute the eventloop
    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllLoops();

    // get and set method 
    bool started() const { return started_; }
    const std::string name() const { return name_; }

    void start(const ThreadInitCallback &cb = ThreadInitCallback());

    void setThreadNum(int numThreads) { numThreads_ = numThreads; }
private:
    EventLoop* baseLoop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;

    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};

#endif