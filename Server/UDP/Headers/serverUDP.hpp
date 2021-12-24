#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <functional>
#include "socketUDP.hpp"

using Processing = std::function<std::string(std::string)>;

class ServerUDP{
public:
    ServerUDP(size_t ip, size_t port);

    ServerUDP() = delete;
    ServerUDP(const ServerUDP&) = delete;

    [[noreturn]] void processing(const Processing& handler);
private:
    SocketUDP listener_;
};

#endif // UDP_SERVER_HPP