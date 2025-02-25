#include <cassert>

#include "GameUtils.hpp"
#include "MessageDeserializer.hpp"

namespace Game {

    Tile MessageDeserializer::deserialize_tile_info(const Message& msg){
        const auto& data = msg.get_data();
        uint8_t x_pos = data[0];
        uint8_t y_pos = data[1];
        TileType type_ = static_cast<TileType>(data[2]);
        uint8_t word_len = data[3];
        assert(word_len < Game::MAX_WORD_SIZE);
        std::string word(data.begin() + 4, data.begin() + 4 + word_len);

        return Tile(x_pos, y_pos, word, type_, false);
    }

    Clue MessageDeserializer::deserialize_clue(const Message& msg){
        const auto& data = msg.get_data();
        uint8_t clue_len = data[0];
        assert(clue_len < Game::MAX_WORD_SIZE);
        std::string clue_str(data.begin() + 1, data.begin() + 1 + clue_len);
        uint8_t num_matches = data[1];

        return { .clue_word=clue_str, .num_matches=num_matches };
    }

    Guess MessageDeserializer::deserialize_guess(const Message& msg){
        const auto& data = msg.get_data();
        uint8_t x_coord = data[0];
        uint8_t y_coord = data[1];

        return { .x_coord=x_coord, .y_coord=y_coord };
    }

    PlayerInfo MessageDeserializer::deserialize_player_info(const Message& msg){
        const auto& data = msg.get_data();
        Team team = static_cast<Team>(data[0]);
        Role role = static_cast<Role>(data[1]);

        return { .team=team, .role=role };
    }

    TileRequest MessageDeserializer::deserialize_tile_request(const Message& msg){
        const auto& data = msg.get_data();
        uint8_t x_coord = data[0];
        uint8_t y_coord = data[1];

        return { .x_coord=x_coord, .y_coord=y_coord };
    }

    Team MessageDeserializer::deserialize_end_of_game(const Message& msg){
        const auto& data = msg.get_data();
        return static_cast<Team>(data[0]);
    }

    bool MessageDeserializer::deserialize_start_of_game(const Message& msg){
        return true;
    }

    GameState MessageDeserializer::deserialize_game_state(const Message& msg){
        const auto& data = msg.get_data();
        Team team = static_cast<Team>(data[0]);
        Role role = static_cast<Role>(data[1]);

        return { .team=team, .role=role };
    }

}
