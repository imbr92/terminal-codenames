#pragma once

#include <string>
#include <vector>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

namespace Game {
    constexpr size_t VERTICAL_BORDER_WIDTH = 2; // [tile] VBW [tile]
    constexpr size_t HORIZONTAL_BORDER_WIDTH = 1; // [tile] \n HBW \n [tile]
    constexpr size_t TILE_ROWS = 5;
    constexpr size_t MAX_WORD_SIZE = 15;
    constexpr size_t TILE_COLUMNS = MAX_WORD_SIZE + 4;

    void center_text(ncpp::Plane& plane, const std::string &text);

    std::array<size_t, 2> get_raw_tile_coordinates(size_t tile_row, size_t tile_column);

    // TODO: define or split out into separate class
    std::vector<std::string> get_words(size_t seed, size_t num_words);
};
