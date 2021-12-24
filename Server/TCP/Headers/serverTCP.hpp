#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <functional>
#include <set>

#include "socketTCP.hpp"

using Processing = std::function<std::string(std::string)>;
inline std::string formPackage(std::string&& message);

class ServerTCP{
public:
    ServerTCP(size_t ip, size_t port);

    ServerTCP() = delete;
    ServerTCP(const ServerTCP&) = delete;

    void listen();
    ServerTCP& accept();
    ServerTCP& select();
    ServerTCP& serveReadyClients(const Processing& handler);

    [[noreturn]] void processing(const Processing& handler);
private:
    SocketTCP listener_;

    std::function<bool(const SocketTCP&, const SocketTCP&)> compare_ =
            [](const SocketTCP& socket_1, const SocketTCP& socket_2)->bool
            {return socket_1.descriptor() > socket_2.descriptor();};

    std::set<SocketTCP, decltype(compare_)> clients_{compare_};

    fd_set readset_{};
    timeval timeout_{760, 0};
    static constexpr size_t backlog_ = 10;
};

#endif // TCP_SERVER_HPP
