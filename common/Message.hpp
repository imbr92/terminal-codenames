#pragma once

#include "GameUtils.hpp"

namespace Game{

    class Message {

    private:
        uint8_t version;
        MessageType type;
        uint32_t packet_len;
        std::vector<char> data;

    public:
        Message(char buf[BUFFER_SIZE]);
        MessageType get_type() const;
        const std::vector<char>& get_data() const;
    };
}
