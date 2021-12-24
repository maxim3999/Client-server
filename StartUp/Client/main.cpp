#include <iostream>

#include "socketUDP.hpp"
#include "socketTCP.hpp"

enum Protocol{
    UDP = SOCK_DGRAM,
    TCP = SOCK_STREAM
};

char msg[] = "200 apples, 30 bananas, 55 peaches and 81 watermelon\n";

int main(int argc, char *argv[]){
    Protocol protocol = Protocol::TCP;
    std::string message{msg, sizeof(msg)};
    if(protocol == Protocol::UDP){
        SocketUDP socket{INADDR_LOOPBACK, 8888};
        socket.send(message);

        socket.close();

        socket = SocketUDP{INADDR_ANY, 7777};

        socket.bind();
        auto [packet, server] = socket.recover();
        std::cout << packet << std::endl;
        socket.close();
    }
    else if(protocol == Protocol::TCP){
        int32_t pid = fork();

        SocketTCP socket{INADDR_LOOPBACK, 8888};
        socket.connect();
        socket.send(message);
        message = socket.recover();
        std::cout << "Message to pid " << pid << "\n" << message << "\n------------\n" << std::endl;
    }

}