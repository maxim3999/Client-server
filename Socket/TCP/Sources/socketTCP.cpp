#include <stdexcept>

#include "socketTCP.hpp"

SocketTCP::SocketTCP(size_t ip, size_t port) : SocketTCP(ip, port, socket(AF_INET, SOCK_STREAM, 0)){
    if(descriptor_ < 0)
        throw std::runtime_error(strerror(errno));
}

SocketTCP::SocketTCP(SocketTCP&& socket) noexcept : descriptor_{socket.descriptor_}, addr_{socket.addr_}{
    socket.descriptor_ = -1;
    socket.addr_ = sockaddr_in{};
}


SocketTCP& SocketTCP::operator=(SocketTCP&& other) noexcept {
    descriptor_ = other.descriptor_;
    addr_ = other.addr_;

    other.descriptor_ = -1;
    other.addr_ = sockaddr_in{};
    return *this;
}

SocketTCP::~SocketTCP(){
    if(descriptor_ >= 0)
        ::close(descriptor_);
}

void SocketTCP::close(){
    if(descriptor_ >= 0){
        ::close(descriptor_);
        addr_ = sockaddr_in{};
    }
}

[[nodiscard]] std::string SocketTCP::recover() const{
    char buf[buffer_size_];
    int total_read = 0;
    while(true){
        int32_t bytes_read = recv(descriptor_, buf + total_read, buffer_size_ - total_read, 0);
        if(bytes_read < 0){
            printf("\n%s\n", (strerror(errno)));
            return std::string{};
        }
        total_read += bytes_read;
        if(buf[total_read - 1] == '\0')
            break;
    }
    return {buf, static_cast<size_t>(total_read - 1)};
}

void SocketTCP::send(std::string& message) const{
    int32_t total_send = 0;
    int32_t bytesleft = message.size();

    while(total_send < message.size()){
        int bytes_send = ::send(descriptor_, message.data() + total_send, bytesleft, 0);
        if(bytes_send == -1)
            printf("\n%s\n", (strerror(errno)));
        total_send += bytes_send;
        bytesleft -= bytes_send;
    }
}

[[nodiscard]] SocketTCP SocketTCP::accept() const{
    return {addr_.sin_addr.s_addr, addr_.sin_port, ::accept(descriptor_, nullptr, nullptr)};
}

void SocketTCP::connect(){
    if(::connect(descriptor_, (struct sockaddr*)&addr_, sizeof(addr_)) < 0)
        throw std::runtime_error(strerror(errno));
}

void SocketTCP::bind(){
    if(::bind(descriptor_, reinterpret_cast<const sockaddr *>(&addr_), sizeof(addr_)) < 0){
        ::close(descriptor_);
        throw std::runtime_error(strerror(errno));
    }
}

[[nodiscard]] int32_t SocketTCP::descriptor() const{
    return descriptor_;
}

SocketTCP::SocketTCP(size_t ip, size_t port, int32_t descriptor) : descriptor_{descriptor}{
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(ip);
    addr_.sin_port = htons(port);
}