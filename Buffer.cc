#include "Buffer.h"

#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

Buffer::Buffer(size_t initSize)
       : buffer_(kCheapPrepend + initSize)
       , readerIndex_(kCheapPrepend)
       , writerIndex_(kCheapPrepend)
{}

/**
 * read data from the file descripted by the fd. use readv function
 * to store data in multiple buffers to gain strage efficiency.
 **/
ssize_t Buffer::readFd(int fd, int* saveErrno)
{
    char extrabuf[65536] = {0}; //64kb
    struct iovec vec[2];

    const size_t writeable = writeableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writeable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovcont = (writeable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcont);

    if (n < 0) {
        *saveErrno = errno;
    }else if (n <= writeable){
        writerIndex_ += n;
    }else {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writeable);
    }
}

ssize_t Buffer::writeFd(int fd, int* saveErrno)
{
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0) {
        *saveErrno = errno;
    }
    return n;
}