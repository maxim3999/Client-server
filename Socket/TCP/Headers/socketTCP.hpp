#ifndef TCP_SOCKET_HPP
#define TCP_SOCKET_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <cstring>

class SocketTCP{
private:
    SocketTCP(size_t ip, size_t port, int32_t descriptor);
public:
    SocketTCP(size_t ip, size_t port);

    SocketTCP() = delete;
    SocketTCP(const SocketTCP&) = delete;

    SocketTCP(SocketTCP&& socket) noexcept;
    SocketTCP& operator=(SocketTCP&& other) noexcept;

    ~SocketTCP();

    void close();

    [[nodiscard]] std::string recover() const;
    void send(std::string& message) const;

    [[nodiscard]] SocketTCP accept() const;
    void connect();
    void bind();

    [[nodiscard]] int32_t descriptor() const;
private:
    int32_t descriptor_{};
    sockaddr_in addr_{};

    static constexpr size_t buffer_size_ = 1024;
};

#endif // TCP_SOCKET_HPP
