#ifndef E_S_SOCKET_H
#define E_S_SOCKET_H

#include "noncopyable.h"

class InetAddress;

class Socket : noncopyable
{
public:
    explicit Socket(int sockfd);
    ~Socket();

    void bindAddress(const InetAddress& localaddr);
    int accept(InetAddress* peeraddr);
    void listen();

    void shutdownWrite();
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

    int fd() const { return sockfd_; }
private:
    const int sockfd_;
};



#endif