#ifndef E_S_THREAD_H
#define E_S_THREAD_H

#include "noncopyable.h"

#include <unistd.h>
#include <functional>
#include <thread>
#include <memory>
#include <string>
#include <atomic>

class Thread : noncopyable
{
public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(ThreadFunc, const std::string& name = std::string());
    ~Thread();

    void start();
    void join();

    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    const std::string& name() const { return name_; }
    static int numCreated() { return numCreated_; }
private:
    bool started_;
    bool joined_;

    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    std::shared_ptr<std::thread> thread_;
    static std::atomic_int numCreated_;

    void setDefaultName();
};



#endif