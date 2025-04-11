#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "../common/Board.hpp"
#include "../common/Message.hpp"
#include "../common/MessageSerializer.hpp"
#include "../common/MessageDeserializer.hpp"

namespace Game {


    template<size_t BOARD_NROWS, size_t BOARD_NCOLS>
    class Client{

    using Grid = std::array<std::array<Tile, BOARD_NCOLS>, BOARD_NROWS>;

    private:

        // has game started?
        bool is_started;

        // PlayerInfo for this client
        PlayerInfo player_info;

        // Game board as visible to this client
        Board<BOARD_NROWS, BOARD_NCOLS> board;

        // Poll Fd for client to server connection
        struct pollfd poll_fd;

        // Std plane
        std::shared_ptr<ncpp::Plane> stdplane;

    public:

        Client(const char* address, const in_port_t& port, std::shared_ptr<ncpp::Plane> plane): board(false){
            is_started = false;

            stdplane = plane;

            int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
            if(sock_fd < 0){
                std::cerr << "[Error] Failed to create socket\n";
                exit(EXIT_FAILURE);
            }

            struct sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            if(inet_pton(AF_INET, address, &server_addr.sin_addr) <= 0){
                std::cerr << "[Error] Invalid server address\n";
                close(sock_fd);
                exit(EXIT_FAILURE);
            }

            if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
                std::cerr << "[Error] Failed to connect to server\n";
                close(sock_fd);
                exit(EXIT_FAILURE);
            }

            // Makes sock_fd nonblocking
            fcntl(sock_fd, F_SETFL, O_NONBLOCK);

            poll_fd.fd = sock_fd;
            // TODO: Figure out if I actually need POLLOUT here?
            poll_fd.events = POLLIN | POLLOUT;
        }

        // Updates board in a way such that it is pushed to ncpp
        void update_board(const Grid& dummy_board){
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    board.set_revealed(i, j, dummy_board[i][j].is_revealed());
                    board.set_type(i, j, dummy_board[i][j].get_type());
                    board.set_word(i, j, dummy_board[i][j].get_word());
                }
            }
            board.draw();
        }

        // TODO: Maybe remove?
        // TODO: Updates full game state in a way such that it is pushed to ncpp
        void update_state(Selection s){

        }

        void draw_player_info(){
            int y = 3;
            int x = 110;
            stdplane->putstr(y, x, "Player Info");
            stdplane->putstr(y + 1, x, to_string(player_info.team).c_str());
            stdplane->putstr(y + 2, x, to_string(player_info.role).c_str());
        }

        void draw_game_state(){
            int y = 7;
            int x = 110;
            stdplane->putstr(y, x, "Game State");
            const auto& cur_game_state = board.get_game_state();
            stdplane->putstr(y + 1, x, to_string(cur_game_state.team).c_str());
            stdplane->putstr(y + 2, x, to_string(cur_game_state.role).c_str());
        }

        void draw_clue(){
            int y = 11;
            int x = 110;
            const auto& cur_clue = board.get_clue();
            stdplane->putstr(y, x, ("Clue: " + cur_clue.clue_word).c_str());
            stdplane->putstr(y + 1, x, (std::string("Num Matches: ") + std::to_string(cur_clue.num_matches)).c_str());
        }

        void draw_winner(const Team& winner){
            int y = 14;
            int x = 110;
            const auto& cur_clue = board.get_clue();
            stdplane->putstr(y, x, ("Winner: " + to_string(winner)).c_str());
        }

        void draw(){
            if(is_started){
                draw_player_info();
                draw_game_state();
                draw_clue();
            }
            board.draw();
        }

        int poll(int __timeout){
            return ::poll(&poll_fd, 1, __timeout);
        }

        short poll_events(short event){
            return poll_fd.revents & event;
        }

        // Return true if it is a reasonable time for this client to provide text input.
        bool allow_text_input(){
            const auto& cur_game_state = board.get_game_state();
            if(
                (cur_game_state.team == player_info.team) &&
                (cur_game_state.role == player_info.role) &&
                (cur_game_state.role == Role::SENDER)
            ){
                return true;
            }
            else return false;
        }

        void process_message(){
            char buffer[BUFFER_SIZE] = {0};
            ssize_t total_bytes_read = 0;
            while(total_bytes_read < FIXED_PACKET_LENGTH){
                ssize_t bytes_read = read(poll_fd.fd, buffer + total_bytes_read, sizeof(buffer) - total_bytes_read);
                total_bytes_read += bytes_read;
                // Do something here since rn if nothing arrives --> infinite loop :O
                if(bytes_read <= 0){
                    close(poll_fd.fd);
                    std::cerr << "[Info] Server disconnected\n";
                    exit(EXIT_FAILURE);
                }
            }
            std::cerr << "Processed Packet\n";
            for(int i = 0; i < FIXED_PACKET_LENGTH; ++i){
                std::cerr << ((int) buffer[i]) << ' ';
            }
            std::cerr << '\n';

            Message raw_msg(buffer);
            MessageType msg_type = raw_msg.get_type();
            std::cerr << "[Info] Received message's type: " << (int) msg_type << '\n';
            apply_message(msg_type, raw_msg);

        }

        void apply_message(MessageType msg_type, const Message& raw_msg){

            if(msg_type != MessageType::PLAYER_INFO && !is_started){
                return;
            }

            if(msg_type == MessageType::TILE_INFO){
                std::cerr << "[Info] Received TILE_INFO message from server\n";
                Tile updated_tile = MessageDeserializer::deserialize_tile_info(raw_msg);
                std::cerr << "[Info] Finished deserializing tile\n";
                std::cerr << "Tile: " << updated_tile.get_x() << ' ' << updated_tile.get_y() << ' ' << ((int)updated_tile.get_type()) << ' ' << updated_tile.get_revealed() << '\n';
                board.set_tile(updated_tile);
                std::cerr << "[Info] Finished setting tile\n";
                board.draw();
                std::cerr << "[Info] Finished processing TILE_INFO message\n";
            }
            else if(msg_type == Game::MessageType::CLUE){
                Game::Clue new_clue = MessageDeserializer::deserialize_clue(raw_msg);
                board.set_clue(new_clue);
                draw_clue();
            }
            else if(msg_type == Game::MessageType::GUESS){
                std::cerr << "[Info] Received guess as a client. Ignoring...\n";
            }
            else if(msg_type == Game::MessageType::PLAYER_INFO){
                std::cerr << "[Info] Received PLAYER_INFO\n";
                player_info = MessageDeserializer::deserialize_player_info(raw_msg);
                std::cerr << "[Info] PlayerInfo: {" << (int) player_info.team << ", " << (int) player_info.role << "}\n";
                is_started = true;
                draw();
            }
            else if(msg_type == Game::MessageType::REQUEST_TILE){
                std::cerr << "[Info] Received request_tile as a client. Ignoring...\n";
            }
            else if(msg_type == Game::MessageType::END_OF_GAME){
                Team winner = MessageDeserializer::deserialize_end_of_game(raw_msg);
                draw_winner(winner);
            }
            else if(msg_type == Game::MessageType::START_OF_GAME){
                // player_info message acts as a start of game for client
                std::cerr << "[Info] Received start_of_game message from server. Ignoring...\n";
            }
            else if(msg_type == Game::MessageType::GAME_STATE){
                GameState game_state = MessageDeserializer::deserialize_game_state(raw_msg);
                board.update_game_state(game_state);
                draw_game_state();
            }
            else{
                std::cerr << "[Error] Received unknown message type\n";
            }
        }

        void start_game(){
            char buf[BUFFER_SIZE];
            MessageSerializer::serialize(buf, true);
            send_all(poll_fd, buf);
            std::cerr << "[Info] Sent start_game successfully\n";
        }

        void send_clue(const std::string& clue_with_count){
            size_t pos = clue_with_count.find_last_of(' ');
            if(pos == std::string::npos){
                std::cerr << "[Info] Clue in wrong format, ignoring...\n";
                return;
            }
            std::string word = clue_with_count.substr(0, pos);
            // TODO: Add error checking here
            size_t num_matches = std::stoi(clue_with_count.substr(pos + 1));

            char buf[BUFFER_SIZE];
            MessageSerializer::serialize(buf, Clue{.clue_word=word, .num_matches=num_matches});
            send_all(poll_fd, buf);
            std::cerr << "sent clue successfully\n";
        }


        // TODO: Updates full game state in a way such that it is pushed to ncpp
        void update_position(int32_t dx, int32_t dy){
            board.update_position(dx, dy);
        }

        void guess_tile(){
            const auto& cur_game_state = board.get_game_state();
            if(
                (cur_game_state.team == player_info.team) &&
                (cur_game_state.role == player_info.role) &&
                (cur_game_state.role == Role::RECEIVER)
            ){
                char buf[BUFFER_SIZE];
                auto guess = Guess{.x_coord=board.get_x(), .y_coord=board.get_y()};
                MessageSerializer::serialize(buf, guess);
                send_all(poll_fd, buf);
                std::cerr << "[Info] Sent guess" << Game::to_string(guess) << '\n';
            }
        }

    };
}
