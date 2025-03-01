#include "MessageSerializer.hpp"
#include <cstring>

namespace Game {
    template<typename T>
    void MessageSerializer::append(char* buf, const T& src, uint32_t& packet_size){
        memcpy(buf + packet_size, reinterpret_cast<const char*>(&src), sizeof(T));
        packet_size += sizeof(T);
    }

    template<typename T>
    void MessageSerializer::append_str(char* buf, const T& src, uint32_t& packet_size){
        memcpy(buf + packet_size, reinterpret_cast<const char*>(src), strlen(src));
        packet_size += strlen(src);
    }

    void MessageSerializer::serialize(char (&buf)[BUFFER_SIZE], const Tile& tile){
        // Packet size starts at 6 to account for header
        uint32_t packet_size = 6;
        std::memset(buf, 0, BUFFER_SIZE);

        uint8_t x = static_cast<uint8_t>(tile.get_x());
        MessageSerializer::append(buf, x, packet_size);

        uint8_t y = static_cast<uint8_t>(tile.get_y());
        MessageSerializer::append(buf, y, packet_size);

        TileType type = tile.get_type();
        MessageSerializer::append(buf, type, packet_size);

        std::string word = tile.get_word();
        uint8_t word_len = static_cast<uint8_t>(word.size());
        MessageSerializer::append(buf, word_len, packet_size);
        MessageSerializer::append_str(buf, word.c_str(), packet_size);

        buf[0] = CURRENT_VERSION;
        uint32_t network_packet_size = htonl(packet_size);
        std::memcpy(buf + 1, reinterpret_cast<char*>(&network_packet_size), sizeof(packet_size));
        buf[5] = static_cast<char>(MessageType::TILE_INFO);
    }

    void MessageSerializer::serialize(char (&buf)[BUFFER_SIZE], const Clue& clue){
        // Packet size starts at 6 to account for header
        uint32_t packet_size = 6;
        std::memset(buf, 0, BUFFER_SIZE);

        const std::string& clue_word = clue.clue_word;
        uint8_t clue_len = static_cast<uint8_t>(clue_word.size());
        MessageSerializer::append(buf, clue_len, packet_size);
        MessageSerializer::append_str(buf, clue_word.c_str(), packet_size);

        uint8_t num_matches = static_cast<uint8_t>(clue.num_matches);
        MessageSerializer::append(buf, num_matches, packet_size);

        buf[0] = CURRENT_VERSION;
        uint32_t network_packet_size = htonl(packet_size);
        std::memcpy(buf + 1, reinterpret_cast<char*>(&network_packet_size), sizeof(packet_size));
        buf[5] = static_cast<char>(MessageType::CLUE);
    }

    void MessageSerializer::serialize(char (&buf)[BUFFER_SIZE], const Guess& guess){
        // Packet size starts at 6 to account for header
        uint32_t packet_size = 6;
        std::memset(buf, 0, BUFFER_SIZE);

        uint8_t x = static_cast<uint8_t>(guess.x_coord);
        MessageSerializer::append(buf, x, packet_size);

        uint8_t y = static_cast<uint8_t>(guess.y_coord);
        MessageSerializer::append(buf, y, packet_size);

        buf[0] = CURRENT_VERSION;
        uint32_t network_packet_size = htonl(packet_size);
        std::memcpy(buf + 1, reinterpret_cast<char*>(&network_packet_size), sizeof(packet_size));
        buf[5] = static_cast<char>(MessageType::GUESS);
    }

    void MessageSerializer::serialize(char (&buf)[BUFFER_SIZE], const PlayerInfo& player_info){
        // Packet size starts at 6 to account for header
        uint32_t packet_size = 6;
        std::memset(buf, 0, BUFFER_SIZE);

        MessageSerializer::append(buf, player_info.team, packet_size);
        MessageSerializer::append(buf, player_info.role, packet_size);

        buf[0] = CURRENT_VERSION;
        uint32_t network_packet_size = htonl(packet_size);
        std::memcpy(buf + 1, reinterpret_cast<char*>(&network_packet_size), sizeof(packet_size));
        buf[5] = static_cast<char>(MessageType::PLAYER_INFO);
    }

    void MessageSerializer::serialize(char (&buf)[BUFFER_SIZE], const TileRequest& tile_request){
        // Packet size starts at 6 to account for header
        uint32_t packet_size = 6;
        std::memset(buf, 0, BUFFER_SIZE);

        uint8_t x = static_cast<uint8_t>(tile_request.x_coord);
        MessageSerializer::append(buf, x, packet_size);

        uint8_t y = static_cast<uint8_t>(tile_request.y_coord);
        MessageSerializer::append(buf, y, packet_size);

        buf[0] = CURRENT_VERSION;
        uint32_t network_packet_size = htonl(packet_size);
        std::memcpy(buf + 1, reinterpret_cast<char*>(&network_packet_size), sizeof(packet_size));
        buf[5] = static_cast<char>(MessageType::REQUEST_TILE);
    }

    void MessageSerializer::serialize(char (&buf)[BUFFER_SIZE], const Team& team){
        // Packet size starts at 6 to account for header
        uint32_t packet_size = 6;
        std::memset(buf, 0, BUFFER_SIZE);

        MessageSerializer::append(buf, team, packet_size);

        buf[0] = CURRENT_VERSION;
        uint32_t network_packet_size = htonl(packet_size);
        std::memcpy(buf + 1, reinterpret_cast<char*>(&network_packet_size), sizeof(packet_size));
        buf[5] = static_cast<char>(MessageType::END_OF_GAME);
    }

    void MessageSerializer::serialize(char (&buf)[BUFFER_SIZE], const bool& start_of_game){
        if(!start_of_game) return;
        // Packet size starts at 6 to account for header
        uint32_t packet_size = 6;
        std::memset(buf, 0, BUFFER_SIZE);

        buf[0] = CURRENT_VERSION;
        uint32_t network_packet_size = htonl(packet_size);
        std::memcpy(buf + 1, reinterpret_cast<char*>(&network_packet_size), sizeof(packet_size));
        buf[5] = static_cast<char>(MessageType::START_OF_GAME);
    }

    void MessageSerializer::serialize(char (&buf)[BUFFER_SIZE], const GameState& game_state){
        // Packet size starts at 6 to account for header
        uint32_t packet_size = 6;
        std::memset(buf, 0, BUFFER_SIZE);

        MessageSerializer::append(buf, game_state.team, packet_size);
        MessageSerializer::append(buf, game_state.role, packet_size);

        buf[0] = CURRENT_VERSION;
        uint32_t network_packet_size = htonl(packet_size);
        std::memcpy(buf + 1, reinterpret_cast<char*>(&network_packet_size), sizeof(packet_size));
        buf[5] = static_cast<char>(MessageType::GAME_STATE);
    }

}
