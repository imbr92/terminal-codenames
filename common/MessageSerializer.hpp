#pragma once

#include "GameUtils.hpp"
#include "Tile.hpp"

namespace Game {

    class MessageSerializer{
    private:
        template<typename T>
        static void append(char* buf, const T& src, uint32_t& packet_size);

    public:
        MessageSerializer() = delete;
        static void serialize(char (&buf)[BUFFER_SIZE], const Tile& tile);
        static void serialize(char (&buf)[BUFFER_SIZE], const Clue& clue);
        static void serialize(char (&buf)[BUFFER_SIZE], const Guess& guess);
        static void serialize(char (&buf)[BUFFER_SIZE], const PlayerInfo& player_info);
        static void serialize(char (&buf)[BUFFER_SIZE], const TileRequest& tile_request);
        static void serialize(char (&buf)[BUFFER_SIZE], const Team& team);
        static void serialize(char (&buf)[BUFFER_SIZE], const bool& start_of_game);

    };
}
