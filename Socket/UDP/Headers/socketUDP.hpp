#ifndef UDP_SOCKET_HPP
#define UDP_SOCKET_HPP

#include <netinet/in.h>

#include <string>

struct sockaddr_in;

class SocketUDP{
private:
    SocketUDP(size_t ip, size_t port, int32_t descriptor);
public:
    SocketUDP(size_t ip, size_t port);

    SocketUDP() = delete;
    SocketUDP(const SocketUDP&) = delete;

    SocketUDP(SocketUDP&& other) noexcept;
    SocketUDP& operator=(SocketUDP&& other) noexcept;

    ~SocketUDP();

    void close();

    SocketUDP& bind();

    [[nodiscard]] std::pair<std::string, sockaddr_in> recover() const;
    int32_t send(const std::string& message);

    [[nodiscard]] int32_t descriptor() const;
private:
    int32_t descriptor_{};
    sockaddr_in addr_{};

    static constexpr size_t buffer_size_ = 1024;
};



#endif // UDP_SOCKET_HPP
