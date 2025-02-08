#include <array>
#include <string>

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

};
