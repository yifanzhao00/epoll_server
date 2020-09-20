#include "InetAddress.h"

#include <strings.h>

InetAddress::InetAddress(u_int16_t port, std::string ip)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET; // ipv4
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const sockaddr_in &addr)
{
    addr_ = addr;
}

std::string InetAddress::topIp() const
{
    char buf[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
    return buf;
} 

std::string InetAddress::toIpPort() const
{
    //ip:port
    std::string buf = topIp();
    uint16_t port = toPort();
    return buf + ":" + std::to_string(port);
} 

uint16_t InetAddress::toPort() const
{
    return ntohs(addr_.sin_port);
}

