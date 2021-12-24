#include <fcntl.h>

#include "serverTCP.hpp"

ServerTCP::ServerTCP(size_t ip, size_t port) : listener_{ip, port}{};

ServerTCP& ServerTCP::accept(){
    SocketTCP client_socket = listener_.accept();

    FD_SET(client_socket.descriptor(), &readset_);
    fcntl(client_socket.descriptor(), F_SETFL, O_NONBLOCK);
    clients_.emplace(std::move(client_socket));

    return *this;
}

ServerTCP& ServerTCP::select(){
    int32_t mx = std::max(listener_.descriptor(), clients_.begin()->descriptor());
    int32_t sel = ::select(mx + 1, &readset_, nullptr, nullptr, &timeout_);
    if(sel == 0){
        clients_.clear();
        throw std::runtime_error(std::string("Server connect timeout"));
    }else if(sel < 0){
        throw std::runtime_error(strerror(errno));
    }
    return *this;
}

void ServerTCP::listen(){
    int recall = 1;
    setsockopt(listener_.descriptor(), SOL_SOCKET, SO_REUSEADDR, &recall, sizeof(int));
    fcntl(listener_.descriptor(), F_SETFL, O_NONBLOCK);

    listener_.bind();

    if(::listen(listener_.descriptor(), backlog_) < 0)
        throw std::runtime_error(strerror(errno));

    clients_.clear();

    FD_SET(listener_.descriptor(), &readset_);
}

[[noreturn]] void ServerTCP::processing(const Processing& handler){
    listen();
    while(true){
        select().accept().serveReadyClients(handler);
    }
}

ServerTCP& ServerTCP::serveReadyClients(const Processing& handler){
    std::vector<decltype(clients_.begin())> clients_to_remove;
    for(auto client = clients_.begin(); client != clients_.end(); ++client){
        if(FD_ISSET(client->descriptor(), &readset_)){
            std::string message = client->recover();

            message = formPackage(handler(message));

            client->send(message);

            if(!clients_.empty() && client != clients_.end())
                clients_to_remove.emplace_back(client);
        }
    }
    for(auto& client : clients_to_remove){
        FD_CLR(client->descriptor(), &readset_);
        clients_.erase(*client);
    }
    return *this;
}

std::string formPackage(std::string&& message){
    std::string packet{message};
    if(packet.back() != '\0')
        packet += '\0';

    if(packet.size() > 1024)
        throw std::runtime_error(std::string("Packet size > buf size"));

    return packet;
}