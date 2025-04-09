#include <poll.h>
#include <assert.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <random>
#include <string>
#include <iostream>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "GameUtils.hpp"

namespace Game {
    void center_text(ncpp::Plane& plane, const std::string &text){
        size_t height = plane.get_dim_y();
        size_t width = plane.get_dim_x();

        size_t x = (width - text.length()) / 2;
        size_t y = height / 2;

        plane.putstr(y, x, text.c_str());
    }

    std::array<size_t, 2> get_raw_tile_coordinates(size_t tile_row, size_t tile_column){
        return {HORIZONTAL_BORDER_WIDTH * (tile_row + 1) + TILE_ROWS * tile_row, VERTICAL_BORDER_WIDTH * (tile_column + 1) + TILE_COLUMNS * tile_column};
    }

    std::vector<std::string> get_words(size_t seed, size_t num_words){
        std::string word;
        std::vector<std::string> all_words;
        std::ifstream file("../assets/words.txt");
        std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));

        if(file.is_open()){
            while (file >> word){
                all_words.push_back(word);
            }
            file.close();
        }
        else{
            std::cerr << "[Error] Couldn't open words file" << std::endl;
            exit(EXIT_FAILURE);
        }

        // TODO: Switch out for better sampling method
        std::shuffle(all_words.begin(), all_words.end(), rng);

        if(num_words > all_words.size()){
            std::cerr << "[Error] Too few words in word bank\n";
            exit(EXIT_FAILURE);
        }

        return std::vector<std::string>(all_words.begin(), all_words.begin() + num_words);
    }

    std::vector<TileType> get_colors(size_t seed, size_t num_cells){
        assert(num_cells == 25); // TODO: Fix so that this is no longer needed

        std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
        std::vector<TileType> colors(num_cells);
        std::fill(colors.begin(), colors.begin() + 9, TileType::RED);
        std::fill(colors.begin() + 9, colors.begin() + 17, TileType::BLUE);
        std::fill(colors.begin() + 17, colors.begin() + 24, TileType::YELLOW);
        std::fill(colors.begin() + 24, colors.end(), TileType::BLACK);
        for(auto i : colors) std::cerr << (int) i << ' ';
        std::cerr << '\n';
        std::shuffle(colors.begin(), colors.end(), rng);
        return colors;
    }

    std::tuple<uint32_t, uint32_t, uint32_t> get_color_channels(uint32_t color){
        return {
            (color >> 16) & 0xFF,
            (color >> 8) & 0xFF,
            (color) & 0xFF,
        };
    }

    bool operator==(const PlayerInfo& p, const GameState& g){
        return p.team == g.team && p.role == g.role;
    }

    bool operator==(const GameState& g, const PlayerInfo& p){
        return p == g;
    }

    bool operator!=(const PlayerInfo& p, const GameState& g){
        return !(p == g);
    }

    bool operator!=(const GameState& g, const PlayerInfo& p){
        return !(g == p);
    }

    bool send_all(struct pollfd& poll_fd, const char (&buf)[Game::BUFFER_SIZE]) {
        size_t total_sent = 0;
        while(total_sent < Game::BUFFER_SIZE){
            poll_fd.events = POLLOUT;

            int ret = poll(&poll_fd, 1,  -1); // Wait indefinitely
            if(ret <= 0){
                std::cerr << "[Error] Failed to poll\n";
                return false;
            }

            if(poll_fd.revents & POLLOUT){
                ssize_t sent = send(poll_fd.fd, buf + total_sent, Game::BUFFER_SIZE - total_sent, 0);
                if(sent == -1){
                    std::cerr << "[Error] Failed to send\n";
                    return false;
                }
                total_sent += sent;
            }
        }
        poll_fd.events = POLLIN;
        return true;
    }


    #define STRINGIFY(p) case(p): return #p;

    std::ostream& operator<<(std::ostream& out, const Guess &guess){
        return out << '{' << guess.x_coord << ", " << guess.y_coord << '}';
    }

    std::ostream& operator<<(std::ostream& out, const PlayerInfo &player_info){
        return out << '{' << player_info.team << ", " << player_info.role << '}';
    }

    std::ostream& operator<<(std::ostream& out, const GameState &game_state){
        return out << '{' << game_state.team << ", " << game_state.role << '}';
    }

    std::ostream& operator<<(std::ostream& out, const Team &team){
        return out << [team]{
            switch(team){
                STRINGIFY(Team::RED);
                STRINGIFY(Team::BLUE);
                default:
                    return "unknown";
            }
        }();
    }

    std::ostream& operator<<(std::ostream& out, const Role &role){
        return out << [role]{
            switch(role){
                STRINGIFY(Role::SENDER);
                STRINGIFY(Role::RECEIVER);
                default:
                    return "unknown";
            }
        }();
    }

    #undef STRINGIFY

};
