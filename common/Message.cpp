#include <cstring>

#include <iostream>

#include "GameUtils.hpp"
#include "Message.hpp"

namespace Game{

    Message::Message(char buf[BUFFER_SIZE]){
        version = buf[0];

        std::memcpy(&packet_len, buf + 1, sizeof(uint32_t));
        packet_len = ntohl(packet_len);
        std::cerr << "Packet len is " << packet_len << '\n';

        type = static_cast<MessageType>(buf[5]);

        data.assign(buf + 6, buf + packet_len);
        for(int i = 6; i < packet_len; ++i){
            std::cerr << (int) data[i] << ' ';
        }
        std::cerr << '\n';
    }

    MessageType Message::get_type() const{
        return type;
    }

    const std::vector<char>& Message::get_data() const{
        return data;
    }

}
