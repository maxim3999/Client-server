#include <iostream>
#include "serverTCP.hpp"
#include "serverUDP.hpp"

enum Protocol{
    UDP = SOCK_DGRAM,
    TCP = SOCK_STREAM
};

auto process = [](std::string message) -> std::string{
    int32_t sum = 0;
    std::vector<int32_t> digits;

    for(auto it = message.begin(); it != message.end(); ++it){
        if(std::isdigit(*it)){
            std::string digit;
            while(std::isdigit(*it) && it != message.end())
                digit += *it++;
            digits.push_back(std::stoi(digit));
            sum += digits.back();
        }
    }

    if(digits.empty()){
        return message;
    }
    else{
        std::sort(digits.begin(), digits.end());

        std::string packet;
        for(const auto& digit : digits)
            packet += std::to_string(digit) + ' ';

        packet += '\n' + std::to_string(sum);

        return packet;
    }
};

int main(){
    Protocol protocol = Protocol::TCP;

    if(protocol == Protocol::UDP){
        ServerUDP server{INADDR_ANY, 8888};
        server.processing(process);
    }
    else if (protocol == Protocol::TCP){
        ServerTCP server{INADDR_ANY, 8888};
        server.processing(process);
    }
}
