#pragma once

#include <assert.h>
#include <ctime>
#include <poll.h>
#include <random>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "../common/Board.hpp"
#include "../common/Message.hpp"
#include "../common/MessageSerializer.hpp"
#include "../common/MessageDeserializer.hpp"

namespace Game {
    // Maybe move to a #define or something
    constexpr int BACKLOG = 5;

    template<size_t BOARD_NROWS, size_t BOARD_NCOLS>
    class Server{

    using Grid = std::array<std::array<Tile, BOARD_NCOLS>, BOARD_NROWS>;

    private:

        bool is_started;

        GameState game_state;

        Board<BOARD_NROWS, BOARD_NCOLS> game_board;

        std::vector<PlayerInfo> players;

        std::vector<struct pollfd> poll_fds;

    public:

        Server(size_t seed, uint16_t PORT){
            int server_fd;
            struct sockaddr_in server_addr;
            socklen_t addr_len = sizeof(server_addr);

            server_fd = socket(AF_INET, SOCK_STREAM, 0);
            if(server_fd == -1){
                std::cerr << "Failed to create server socket\n";
                exit(EXIT_FAILURE);
            }

            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(PORT);

            int opt = 1;
            if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
                std::cerr << "setsockopt failed\n";
                exit(EXIT_FAILURE);
            }

            if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
                std::cerr << "Bind failed\n";
                exit(EXIT_FAILURE);
            }

            // TODO: How to use this for multiple TCP connections?
            if(listen(server_fd, BACKLOG) == -1){
                std::cerr << "Failed to listen\n";
                close(server_fd);
                exit(EXIT_FAILURE);
            }

            std::cerr << "[Info] Server listening on port " << PORT << "\n";

            // Setting up poll
            poll_fds.push_back(pollfd{server_fd, POLLIN, 0});

            is_started = false;
            game_board = Board<5, 5>(seed);
            game_state = {Game::Team::RED, Game::Role::SENDER};
        }

        Tile get_tile(size_t x_coord, size_t y_coord){
            return game_board.game_state.get_tile(x_coord, y_coord);
        }

        // Returns grid with hidden words
        Grid get_full_grid(){
            return game_board.get_grid();
        }

        // TODO: Is this good design? Board is now used for UI (client movement) and keeping track of server state
        void select(size_t x_coord, size_t y_coord){
            game_board.set_position(x_coord, y_coord);
            game_board.select();
        }

        // Returns grid with only revealed cells filled out
        Grid get_partial_grid(){
            Grid hidden_grid;
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    if(game_board.is_revealed(i, j)){
                        if(game_board.is_revealed(i, j)){
                            hidden_grid[i][j] = Tile(i, j, game_board.get_word(i, j), game_board.get_type(i, j), true);
                        }
                        else{
                            hidden_grid[i][j] = Tile(i, j, "", TileType::UNKNOWN, false);
                        }
                    }
                }
            }
            return hidden_grid;
        }

        // TODO: expand this for more than 4 players later
        void assign_players(size_t num_players){
            assert(num_players == 4);
            players.reserve(num_players);
            players.push_back(PlayerInfo{Team::RED, Role::SENDER});
            players.push_back(PlayerInfo{Team::RED, Role::RECEIVER});
            players.push_back(PlayerInfo{Team::BLUE, Role::SENDER});
            players.push_back(PlayerInfo{Team::BLUE, Role::RECEIVER});

            std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
            std::shuffle(players.begin(), players.end(), rng);

            //std::uniform_int_distribution<int> dist(1, 100);
        }

        const std::vector<PlayerInfo>& get_assignment(){
            return players;
        }

        int poll(){
            return ::poll(poll_fds.data(), poll_fds.size(), -1);
        }

        // TODO: Support the following:
        // Send board
        // Receive clue
        // Send clue
        // Handle connections

        void process_connection(){
            if(is_started){
                std::cerr << "[Info] Incoming client connection after game start. Ignoring...\n";
                return;
            }

            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int client_fd = accept(poll_fds[0].fd, (struct sockaddr*) &client_addr, &addr_len);
            if(client_fd < 0){
                std::cerr << "[Warn] Failed to accept client\n";
                return;
            }

            poll_fds.push_back(pollfd{client_fd, POLLIN, 0});
            std::cerr << "[Info] New client connected\n";

        }

        void process_message(size_t client_idx){
            char buffer[BUFFER_SIZE] = {0};
            ssize_t total_bytes_read = 0;
            while(total_bytes_read < FIXED_PACKET_LENGTH){
                ssize_t bytes_read = read(poll_fds[client_idx].fd, buffer + total_bytes_read, sizeof(buffer) - total_bytes_read);
                total_bytes_read += bytes_read;
                // Do something here since rn if nothing arrives --> infinite loop :O
                if(bytes_read <= 0){
                    close(poll_fds[client_idx].fd);
                    std::cerr << "[Info] Client disconnected\n";
                    exit(EXIT_FAILURE);
                }
            }

            Message raw_msg(buffer);
            MessageType msg_type = raw_msg.get_type();
            apply_message(client_idx, msg_type, raw_msg);
        }

        void apply_message(size_t client_idx, MessageType msg_type, const Message& raw_msg){
            // Client Id = Index in poll_fds.

            // TODO: get client team and role --> add checks to make sure they arent speaking for another team, etc.
            if(msg_type != Game::MessageType::START_OF_GAME && !is_started){
                return;
            }

            if(msg_type == Game::MessageType::TILE_INFO){
                std::cerr << "[Info] Received TILE_INFO message from client. Ignoring...\n";
            }
            else if(msg_type == Game::MessageType::CLUE){
                // Add checks here to make sure it is from right team/right turn/etc.
                Game::Clue new_clue = Game::MessageDeserializer::deserialize_clue(raw_msg);
            }
            else if(msg_type == Game::MessageType::GUESS){
                // Add checks here to make sure it is from right team/right turn/etc.
                Game::Guess new_guess = Game::MessageDeserializer::deserialize_guess(raw_msg);
                // TODO make this interface better (don't want to copy all assignments every time
                Game::PlayerInfo current_player = get_assignment()[client_idx];
                if(current_player != game_state){
                    std::cerr << "[Info] Received guess from wrong player or during wrong game state, ignoring...\n";
                }
                //TODO: Continue from here in server
                // apply_guess(client_idx, new_guess);
            }
            else if(msg_type == Game::MessageType::PLAYER_INFO){
                std::cerr << "[Info] Received PLAYER_INFO message from client. Ignoring...\n";
            }
            else if(msg_type == Game::MessageType::REQUEST_TILE){
                Game::TileRequest request = Game::MessageDeserializer::deserialize_tile_request(raw_msg);
                respond_to_request(client_idx, request);
            }
            else if(msg_type == Game::MessageType::END_OF_GAME){
                std::cerr << "[Info] Received END_OF_GAME message from client. Ignoring...\n";
            }
            else if(msg_type == Game::MessageType::START_OF_GAME){
                if(is_started){
                    std::cerr << "[Info] Received START_OF_GAME message from client even though game has already started. Ignoring...\n";
                }
                is_started = true;
                // for now assume exactly 4 players
                assert(poll_fds.size() == 5);
                assign_players(4);
                send_assignments();
            }
            else if(msg_type == Game::MessageType::GAME_STATE){
                std::cerr << "[Info] Received GAME_STATE message from client. Ignoring...\n";
            }
            else{
                std::cerr << "[Error] Received unknown message type\n";
            }

            // do some processing

        }

        bool send_assignments(){
            bool worked = true;
            char buf[Game::BUFFER_SIZE];
            const std::vector<Game::PlayerInfo>& player_info = get_assignment();

            for(size_t i = 0; i < player_info.size(); ++i){
                memset(buf, 0, sizeof(buf));
                Game::MessageSerializer::serialize(buf, player_info[0]);
                worked |= Game::send_all(poll_fds[i + 1], buf);
            }

            return worked;
        }

        bool respond_to_request(size_t client_idx, TileRequest request){
            char buf[Game::BUFFER_SIZE];
            struct pollfd& poll_fd = poll_fds[client_idx];

            // TODO: Add logic to not send full status of tile unless the player is allowed to actually know this
            const Tile& tile = game_board.get_tile(request.x_coord, request.y_coord);
            MessageSerializer::serialize(buf, tile);

            return send_all(poll_fd, buf);
        }

        //TODO: Finish
        bool apply_guess(size_t client_idx, Guess request){
            char buf[Game::BUFFER_SIZE];
            struct pollfd& poll_fd = poll_fds[client_idx];

            game_board.select(request.x_coord, request.y_coord);


            // TODO: Add logic to not send full status of tile unless the player is allowed to actually know this
            Tile tile = game_board.get_tile(request.x_coord, request.y_coord);
            MessageSerializer::serialize(buf, tile);
            return Game::send_all(poll_fd, buf);
        }

        int32_t poll_events(short event){
            if(poll_fds[0].revents & event){
                return 0;
            }

            for(size_t i = 1; i < poll_fds.size(); ++i){
                if(poll_fds[i].revents & event){
                    return i;

                }
            }

            return -1;
        }

    };

}
