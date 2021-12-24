#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

#include "socketUDP.hpp"

SocketUDP::SocketUDP(size_t ip, size_t port) : SocketUDP(ip, port, socket(AF_INET, SOCK_DGRAM, 0)){
    if(descriptor_ < 0)
        throw std::runtime_error(strerror(errno));
}

SocketUDP::SocketUDP(SocketUDP&& other) noexcept : descriptor_{other.descriptor_}, addr_{other.addr_}{
    other.descriptor_ = -1;
    other.addr_ = sockaddr_in{};
}

SocketUDP& SocketUDP::operator=(SocketUDP&& other) noexcept {
    descriptor_ = other.descriptor_;
    addr_ = other.addr_;

    other.descriptor_ = -1;
    other.addr_ = sockaddr_in{};
    return *this;
}

SocketUDP::~SocketUDP(){
    if(descriptor_ >= 0)
        ::close(descriptor_);
}

void SocketUDP::close(){
    if(descriptor_ >= 0){
        ::close(descriptor_);
        addr_ = sockaddr_in{};
    }
}

[[nodiscard]] std::pair<std::string, sockaddr_in> SocketUDP::recover() const{
    char buf[buffer_size_];
    sockaddr_in dest_addr{};
    socklen_t dest_addr_len = sizeof(dest_addr);
    int32_t bytes_recovered = recvfrom(descriptor(), buf, buffer_size_, 0,
                                       reinterpret_cast<sockaddr*>(&dest_addr),
                                       &dest_addr_len);

    if(bytes_recovered < 0)
        throw std::runtime_error(strerror(errno));

    std::string message{buf, static_cast<size_t>(bytes_recovered)};
    return std::make_pair(std::move(message), dest_addr);
}

int32_t SocketUDP::send(const std::string& message){
    if(message.size() < buffer_size_){
        return sendto(descriptor_, message.data(), message.size(), 0,
                      reinterpret_cast<struct sockaddr *>(&addr_), sizeof(addr_));
    }
    else{
        ::close(descriptor_);
        throw std::runtime_error(std::string("Message size > buffer size"));
    }
}

SocketUDP& SocketUDP::bind(){
    if(::bind(descriptor_, reinterpret_cast<const sockaddr *>(&addr_), sizeof(addr_)) < 0){
        ::close(descriptor_);
        throw std::runtime_error(strerror(errno));
    }
    return *this;
}

[[nodiscard]] int32_t SocketUDP::descriptor() const{
    return descriptor_;
}

SocketUDP::SocketUDP(size_t ip, size_t port, int32_t descriptor) : descriptor_(descriptor){
        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = htonl(ip);
        addr_.sin_port = htons(port);
}