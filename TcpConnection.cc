#include "TcpConnection.h"
#include "Logger.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

#include <functional>
#include <errno.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <string>


static EventLoop* CheckLoopNotNull(EventLoop* loop) 
{
    if (loop == nullptr) {
        LOG_FATAL("%s:%s:%d TcpConnection Loop is null! \n", __FILE__, __FUNCTION__, __LINE__);
    } 
    return loop; 
}

TcpConnection::TcpConnection(EventLoop* loop, const std::string &name, int sockfd, 
                             const InetAddress& localAddr, const InetAddress& peerAddr)
              : loop_(CheckLoopNotNull(loop))
              , name_(name)
              , state_(kConnecting)
              , reading_(true)
              , socket_(new Socket(sockfd))
              , channel_(new Channel(loop, sockfd))
              , localAddr_(localAddr)
              , peerAddr_(peerAddr_)
              , highWaterMark_(64 * 1024 * 1024) // 64MB
{
    //register the interested events in channel
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));

    LOG_INFO("TcpConnection::ctor[%s] at fd=%d\n", name_.c_str(), sockfd);

    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_INFO("TcpConnection::dtor[%s] at fd=%d state=%d \n", name_.c_str(), channel_->fd(), (int)state_);
}

void TcpConnection::send(const std::string& buf)
{
    if (state_ == kConnected) {
        if (loop_->isInLoopThread()) {
            sendInLoop(buf.c_str(), buf.size());
        }else {
            loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, buf.c_str(), buf.size()));
        }
    }
}

/**
 * highWaterMark to control the data send rate
 * */
void TcpConnection::sendInLoop(const void* data, size_t len)
{
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;

    if (state_ == kDisconnected)
    {
        LOG_ERROR("disconnected, give up writing!");
        return;
    }

    // first time to write data in the channel and the outputbuffer is empty
    if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {
        nwrote = ::write(channel_->fd(), data, len);
        if (nwrote >= 0) {
            remaining = len - nwrote;
            if (remaining == 0 && writeCompleteCallback_) {
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
        }else {
            nwrote = 0;
            if (errno != EWOULDBLOCK) { //EAGAIN
                LOG_ERROR("Tcpconnection::sendInLoop");
                if (errno == EPIPE || errno == ECONNRESET) {
                    faultError = true;
                }
            }
        }
    }

    // do not send entire data, the remaining data needs to store in the output buffer
    if (!faultError && remaining > 0) {
        size_t oldLen = outputBuffer_.readableBytes();
        if (oldLen + remaining >= highWaterMark_ && oldLen < highWaterMark_ && highWaterMarkCallback_) {
            loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
        }

        outputBuffer_.append(static_cast<const char*>(data) + nwrote, remaining);
        if (!channel_->isWriting()) {
            channel_->enableWriting(); // register write event
        }
    }
}

void TcpConnection::handleRead(TimeStamp receiveTime) 
{
    int saveErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &saveErrno);

    if (n > 0) {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }else if (n == 0) {
        handleClose();
    }else {
        errno = saveErrno;
        LOG_ERROR("TcpConnection::handleRead");
        handleError();
    }
}

void TcpConnection::handleWrite() 
{
    if (channel_->isWriting()) {
        int saveErrno = 0;
        ssize_t n = outputBuffer_.writeFd(channel_->fd(), &saveErrno);

        if (n > 0) {
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0) {
                channel_->disableWriting();
                if (writeCompleteCallback_)
                {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }
                if (state_ == kDisconnecting) {
                    shutdownInLoop();
                }
            }
        }else {
            LOG_ERROR("TcpConnection::handleWrite");
        }
    }else {
        LOG_ERROR("TcpConnection fd=%d is down, no more writing \n", channel_->fd());
    }
}

void TcpConnection::handleClose()
{
    LOG_INFO("TcpConnection::handleClose fd=%d state=%d \n", channel_->fd(), (int)state_);
    setState(kDisconnected);
    channel_->disableAll();

    TcpConnectionPtr connPtr(shared_from_this());
    connectionCallback_(connPtr);
    closeCallback_(connPtr); // TcpServer::removeConnection 
}

void TcpConnection::handleError()
{
    int optval;
    socklen_t optlen = sizeof optval;
    int err = 0;

    if (::getsockopt(channel_->fd(), SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
        err = errno;
    }else {
        err = optval;
    }

    LOG_ERROR("TcpConnection::handleError name:%s - SO_ERROR:%d \n", name_.c_str(), err);
}

void TcpConnection::connectEstablished()
{
    setState(kConnected);
    channel_->tie(shared_from_this());
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed() 
{
    if (state_ == kConnected) {
        setState(kConnected);
        channel_->disableAll();
        connectionCallback_(shared_from_this());
    }

    channel_->remove();
}

void TcpConnection::shutdown()
{
    if (state_ == kConnected) {
        setState(kDisconnected);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this)); 
    }
}

void TcpConnection::shutdownInLoop()
{
    if (!channel_->isWriting()) {
        socket_->shutdownWrite();  //EPOLLHUP
    }
}




