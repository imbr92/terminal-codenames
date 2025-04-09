#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "catch_amalgamated.hpp"

#include "../common/MessageSerializer.hpp"
#include "../common/MessageDeserializer.hpp"

#define CONFIG_CATCH_MAIN

TEST_CASE("TILE_INFO", "[serialization]"){
    char buf[Game::BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    Game::Tile tile(3, 4, "hello", Game::TileType::BLUE, false);
    Game::MessageSerializer::serialize(buf, tile);
    Game::Message raw_msg = Game::Message(buf);
    REQUIRE(raw_msg.get_type() == Game::MessageType::TILE_INFO);

    Game::Tile deserialized_tile = Game::MessageDeserializer::deserialize_tile_info(raw_msg);
    REQUIRE(deserialized_tile.get_x() == tile.get_x());
    REQUIRE(deserialized_tile.get_y() == tile.get_y());
    REQUIRE(deserialized_tile.get_type() == tile.get_type());
    REQUIRE(deserialized_tile.get_word() == tile.get_word());
}

TEST_CASE("CLUE", "[serialization]"){
    char buf[Game::BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    Game::Clue clue = {.clue_word = "Among us", .num_matches=1};
    Game::MessageSerializer::serialize(buf, clue);
    Game::Message raw_msg = Game::Message(buf);
    REQUIRE(raw_msg.get_type() == Game::MessageType::CLUE);

    Game::Clue deserialized_clue = Game::MessageDeserializer::deserialize_clue(raw_msg);
    REQUIRE(deserialized_clue.clue_word == clue.clue_word);
    REQUIRE(deserialized_clue.num_matches == clue.num_matches);
}

TEST_CASE("GUESS", "[serialization]"){
    char buf[Game::BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    Game::Guess guess = {.x_coord=5, .y_coord=6};
    Game::MessageSerializer::serialize(buf, guess);
    Game::Message raw_msg = Game::Message(buf);
    REQUIRE(raw_msg.get_type() == Game::MessageType::GUESS);

    Game::Guess deserialized_guess= Game::MessageDeserializer::deserialize_guess(raw_msg);
    REQUIRE(deserialized_guess.x_coord == guess.x_coord);
    REQUIRE(deserialized_guess.y_coord == guess.y_coord);
}

TEST_CASE("PLAYER_INFO", "[serialization]"){
    char buf[Game::BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    Game::PlayerInfo player_info = {.team=Game::Team::BLUE, .role=Game::Role::SENDER};
    Game::MessageSerializer::serialize(buf, player_info);
    Game::Message raw_msg = Game::Message(buf);
    REQUIRE(raw_msg.get_type() == Game::MessageType::PLAYER_INFO);

    Game::PlayerInfo deserialized_player_info = Game::MessageDeserializer::deserialize_player_info(raw_msg);
    REQUIRE(deserialized_player_info.team == player_info.team);
    REQUIRE(deserialized_player_info.role == player_info.role);
}

TEST_CASE("TILE_REQUEST", "[serialization]"){
    char buf[Game::BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    Game::TileRequest tile_request = {.x_coord=3, .y_coord=2};
    Game::MessageSerializer::serialize(buf, tile_request);
    Game::Message raw_msg = Game::Message(buf);
    REQUIRE(raw_msg.get_type() == Game::MessageType::REQUEST_TILE);

    Game::TileRequest deserialized_tile_request = Game::MessageDeserializer::deserialize_tile_request(raw_msg);
    REQUIRE(deserialized_tile_request.x_coord == tile_request.x_coord);
    REQUIRE(deserialized_tile_request.y_coord == tile_request.y_coord);
}

TEST_CASE("END_OF_GAME", "[serialization]"){
    char buf[Game::BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    Game::Team team = Game::Team::BLUE;
    Game::MessageSerializer::serialize(buf, team);
    Game::Message raw_msg = Game::Message(buf);
    REQUIRE(raw_msg.get_type() == Game::MessageType::END_OF_GAME);

    Game::Team deserialized_team = Game::MessageDeserializer::deserialize_end_of_game(raw_msg);
    REQUIRE(team == deserialized_team);
}

TEST_CASE("START_OF_GAME", "[serialization]"){
    char buf[Game::BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    bool start_of_game = true;
    Game::MessageSerializer::serialize(buf, start_of_game);
    Game::Message raw_msg = Game::Message(buf);
    REQUIRE(raw_msg.get_type() == Game::MessageType::START_OF_GAME);

    bool deserialized_start_of_game = Game::MessageDeserializer::deserialize_start_of_game(raw_msg);
    REQUIRE(start_of_game == deserialized_start_of_game);
}

TEST_CASE("GAME_STATE", "[serialization]"){
    char buf[Game::BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    Game::GameState game_state = {.team=Game::Team::BLUE, .role=Game::Role::SENDER};
    Game::MessageSerializer::serialize(buf, game_state);
    Game::Message raw_msg = Game::Message(buf);
    REQUIRE(raw_msg.get_type() == Game::MessageType::GAME_STATE);

    Game::GameState deserialized_game_state = Game::MessageDeserializer::deserialize_game_state(raw_msg);
    REQUIRE(deserialized_game_state.team == game_state.team);
    REQUIRE(deserialized_game_state.role == game_state.role);
}
