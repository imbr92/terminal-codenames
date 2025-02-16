#pragma once

#include "GameUtils.hpp"
#include "Message.hpp"
#include "Tile.hpp"

namespace Game {

    class MessageDeserializer{

    public:
        MessageDeserializer() = delete;
        static Tile deserialize_tile_info(const Message& msg);
        static Clue deserialize_clue(const Message& msg);
        // TODO: Be careful in deserializing here. Output is size_t, but we only reserve 1 byte for x coord, y coord
        static Guess deserialize_guess(const Message& msg);
        static PlayerInfo deserialize_player_info(const Message& msg);
        // TODO: Be careful in deserializing here. Output is size_t, but we only reserve 1 byte for x coord, y coord
        static TileRequest deserialize_tile_request(const Message& msg);
        static Team deserialize_end_of_game(const Message& msg);
        static bool deserialize_start_of_game(const Message& msg);

    };
}
