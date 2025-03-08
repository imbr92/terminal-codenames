#include <poll.h>

#include <array>
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
        return std::vector<std::string>(num_words, "AMONG US");
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

    std::ostream& operator<<(std::ostream& out, const PlayerInfo &player_info){
        return out << '{' << player_info.team << ", " << player_info.role << '}';
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
