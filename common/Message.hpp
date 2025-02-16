#pragma once

#include "GameUtils.hpp"

namespace Game{

    class Message {

    private:
        uint8_t version;
        MessageType type;
        uint32_t packet_len;
        std::vector<uint8_t> data;

    public:
        Message(char buf[BUFFER_SIZE]);
        MessageType get_type() const;
        const std::vector<uint8_t>& get_data() const;
    };
}
