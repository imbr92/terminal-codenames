#include <cstring>

#include "GameUtils.hpp"
#include "Message.hpp"

namespace Game{

    Message::Message(char buf[BUFFER_SIZE]){
        version = buf[0];

        std::memcpy(&packet_len, buf + 1, sizeof(uint32_t));
        packet_len = ntohl(packet_len);

        type = static_cast<MessageType>(buf[5]);

        data.assign(buf + 6, buf + packet_len);
    }

    MessageType Message::get_type() const{
        return type;
    }

    const std::vector<uint8_t>& Message::get_data() const{
        return data;
    }

}
