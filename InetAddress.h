#ifndef E_S_INETADDRESS
#define E_S_INETADDRESS

#include <arpa/inet.h>
#include <netinet/in.h>

#include <string>

// socket address 
// struct sockaddr_in {
//    short int            sin_family;
//    unsigned short int   sin_port;
//    struct in_addr       sin_addr;
//    unsigned char        sin_zero[8];
// };
class InetAddress
{
public:
    explicit InetAddress(u_int16_t port = 0, std::string ip = "127.0.0.1");
    explicit InetAddress(const sockaddr_in &addr_);

    std::string topIp() const;
    std::string toIpPort() const;
    uint16_t toPort() const;

    const sockaddr_in* getSockaddr() const { return &addr_; }
    void setScockaddr(const sockaddr_in &addr) { addr_ = addr; }

private:
    sockaddr_in addr_;
};



#endif





