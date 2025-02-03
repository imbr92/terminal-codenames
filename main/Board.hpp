#pragma once

#include <array>
#include <vector>

#include "Tile.hpp"
#include "GameUtils.hpp"

namespace Game {

    template<size_t BOARD_NROWS, size_t BOARD_NCOLS>
    class Board{
        using Grid = std::array<std::array<Tile, BOARD_NCOLS>, BOARD_NROWS>;
    private:
        Grid grid;

    public:

        Board(size_t seed){
            const std::vector<std::string> words = get_words(seed, BOARD_NROWS * BOARD_NCOLS);
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    const auto [x, y] = get_raw_tile_coordinates(i, j);
                    grid[i][j] = Tile(x, y, TILE_ROWS, TILE_COLUMNS, words[i * BOARD_NROWS + j]);
                }
            }
        }

    };
}
