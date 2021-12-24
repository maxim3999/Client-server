#include "serverUDP.hpp"

ServerUDP::ServerUDP(size_t ip, size_t port) : listener_{ip, port} {};

[[noreturn]] void ServerUDP::processing(const Processing& handler){
    listener_.bind();
    while(true){
        auto [message, client_addr] = listener_.recover();

        std::string packet = handler(message);

        SocketUDP client{INADDR_LOOPBACK, 7777};
        client.send(packet);
        client.close();
    }
}

