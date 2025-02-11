#pragma once

#include <string>
#include <vector>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#define CHANNEL_INIT_32(cc) \
    (cc | NC_BGDEFAULT_MASK)
#define CHANNEL_INIT_64(fg, bg) \
    ((CHANNEL_INIT_32(fg) << 32ULL) | CHANNEL_INIT_32(bg))

namespace Game {
    constexpr size_t VERTICAL_BORDER_WIDTH = 2; // [tile] VBW [tile]
    constexpr size_t HORIZONTAL_BORDER_WIDTH = 1; // [tile] \n HBW \n [tile]
    constexpr size_t TILE_ROWS = 5;
    constexpr size_t MAX_WORD_SIZE = 15;
    constexpr size_t TILE_COLUMNS = MAX_WORD_SIZE + 4;

    // Color constants
    constexpr uint32_t BLACK = 0x000000;
    constexpr uint32_t WHITE = 0xFFFFFF;

    const uint32_t YELLOW_LIGHT = 0xFFFF80;
    const uint32_t YELLOW_DARK = 0xCCCC00;

    const uint32_t BLUE_LIGHT = 0x66CCFF;
    const uint32_t BLUE_DARK = 0x000066;

    const uint32_t RED_LIGHT = 0xFF6666;
    const uint32_t RED_DARK = 0x990000;

    const uint32_t PURPLE_LIGHT = 0xCC66CC;
    const uint32_t PURPLE_DARK = 0x660066;

    const uint32_t GREY_LIGHT = 0xB0B0B0;
    const uint32_t GREY_MEDIUM = 0x808080;
    const uint32_t GREY_DARK = 0x333333;


    // Default Theme
    constexpr uint64_t DEFAULT_UNSELECTED   = CHANNEL_INIT_64(WHITE, GREY_MEDIUM);
    constexpr uint64_t DEFAULT_SELECTED   = CHANNEL_INIT_64(BLACK, PURPLE_LIGHT);

    // Yellow Theme
    constexpr uint64_t YELLOW_UNSELECTED   = CHANNEL_INIT_64(BLACK, YELLOW_DARK);
    constexpr uint64_t YELLOW_SELECTED   = CHANNEL_INIT_64(BLACK, YELLOW_LIGHT);

    // Red Theme
    constexpr uint64_t RED_UNSELECTED   = CHANNEL_INIT_64(WHITE, RED_DARK);
    constexpr uint64_t RED_SELECTED   = CHANNEL_INIT_64(BLACK, RED_LIGHT);

    // Blue Theme
    constexpr uint64_t BLUE_UNSELECTED   = CHANNEL_INIT_64(WHITE, BLUE_DARK);
    constexpr uint64_t BLUE_SELECTED   = CHANNEL_INIT_64(BLACK, BLUE_LIGHT);

    // Black/Grey Theme
    constexpr uint64_t BLACK_UNSELECTED   = CHANNEL_INIT_64(WHITE, GREY_DARK);
    constexpr uint64_t BLACK_SELECTED   = CHANNEL_INIT_64(BLACK, GREY_LIGHT);


    enum Team {RED, BLUE};
    enum Role {RECEIVER, SENDER};

    struct Selection { size_t x_pos; size_t y_pos; Team team; };

    struct Clue { std::string clue_word; size_t num_matches; };


    void center_text(ncpp::Plane& plane, const std::string &text);

    std::array<size_t, 2> get_raw_tile_coordinates(size_t tile_row, size_t tile_column);

    // TODO: define or split out into separate class
    std::vector<std::string> get_words(size_t seed, size_t num_words);

    std::tuple<uint32_t, uint32_t, uint32_t> get_color_channels(uint32_t color);
};
