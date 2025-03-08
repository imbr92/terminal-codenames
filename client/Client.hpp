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

        // TODO: Support the following:
        // Update game state (with dummy board) -- DONE
        // Receive board (with dummy tiles)
        // Receive Clue (Both clue receiver and sender to receive opponent clues)
        // Send Clue (Only sender)
        // Send Selection (Only receiver)

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

        // TODO: Updates full game state in a way such that it is pushed to ncpp
        void update_state(Selection s){

        }

        void draw_player_info(){
            int y = 0;
            int x = 0;
            stdplane->putstr(y, x, to_string(player_info.team).c_str());
            stdplane->putstr(y + 5, x, to_string(player_info.role).c_str());
        }

        void draw(){
            if(is_started) draw_player_info();
            board.draw();
        }

        int poll(int __timeout){
            return ::poll(&poll_fd, 1, __timeout);
        }

        short poll_events(short event){
            return poll_fd.revents & event;
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
            std::cerr << "Packet\n";
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
                std::cerr << "[Info] Received TILE_INFO message from client. Ignoring...\n";
                Tile updated_tile = MessageDeserializer::deserialize_tile_info(raw_msg);
                board.set_tile(updated_tile);
                board.draw();
            }
            else if(msg_type == Game::MessageType::CLUE){
                // TODO: Add checks here to make sure it is from right team/right turn/etc.
                Game::Clue new_clue = MessageDeserializer::deserialize_clue(raw_msg);
                // TODO: Add call to draw clue (maybe done within board.draw())
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
                // TODO: is this sufficient?
                board.set_winner(winner);
            }
            else if(msg_type == Game::MessageType::START_OF_GAME){
                // player_info message acts as a start of game for client
                std::cerr << "[Info] Received start_of_game message from server. Ignoring...\n";
            }
            else if(msg_type == Game::MessageType::GAME_STATE){
                GameState game_state = MessageDeserializer::deserialize_game_state(raw_msg);
                board.update_game_state(game_state);
            }
            else{
                std::cerr << "[Error] Received unknown message type\n";
            }
        }

        void start_game(){
            char buf[BUFFER_SIZE];
            MessageSerializer::serialize(buf, true);
            send_all(poll_fd, buf);
            std::cerr << "send successfully\n";

        }


        // TODO: Updates full game state in a way such that it is pushed to ncpp
        void update_position(int32_t dx, int32_t dy){
            board.update_position(dx, dy);
        }

        void select(){
            board.select();
        }

    };
}
