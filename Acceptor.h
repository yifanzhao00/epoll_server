#ifndef E_S_ACCEPTOR_H
#define E_S_ACCEPTOR_H

#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"


#include <functional>

class EventLoop;
class InetAddress;

class Acceptor : noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;

    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
    ~Acceptor();

    void listen();

    void setNewConnectionCallback(const NewConnectionCallback &cb) { NewConnectionCallback_ = cb; }
    bool listenning() const { return listenning_; }
private:
    EventLoop* loop_; //baseloop
    
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback NewConnectionCallback_;
    bool listenning_;

    void handleRead();
};




#endif