#include <cstddef>
#include <array>

template<size_t ROW_TILES, size_t COL_TILES>
class Board{
    // TODO: figure out if I actually want this to be a static constexpr data member
    static constexpr size_t VERTICAL_BORDER_WIDTH = 2; // [tile] VBW [tile]
    static constexpr size_t HORIZONTAL_BORDER_WIDTH = 1; // [tile] \n HBW \n [tile]
    static constexpr size_t TILE_ROWS = 5;
    static constexpr size_t MAX_WORD_SIZE = 15;
    static constexpr size_t TILE_COLUMNS = MAX_WORD_SIZE + 4;

    // ncpp::Plane

    std::array<size_t, 2> get_terminal_coordinates(size_t tile_row, size_t tile_column){
        return {HORIZONTAL_BORDER_WIDTH * (tile_row + 1) + TILE_ROWS, VERTICAL_BORDER_WIDTH * (tile_column + 1) + TILE_COLUMNS};
    }


};
