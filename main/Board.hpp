#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include "Tile.hpp"
#include "GameUtils.hpp"

namespace Game {

    template<size_t BOARD_NROWS, size_t BOARD_NCOLS>
    class Board{
        using Grid = std::array<std::array<Tile, BOARD_NCOLS>, BOARD_NROWS>;
    private:
        int32_t x_pos, y_pos;
        Grid grid;

    public:

        Board(size_t seed){
            x_pos = 0, y_pos = 0;
            const std::vector<std::string> words = get_words(seed, BOARD_NROWS * BOARD_NCOLS);
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    const auto [x, y] = get_raw_tile_coordinates(i, j);
                    grid[i][j] = Tile(x, y, TILE_ROWS, TILE_COLUMNS, words[i * BOARD_NROWS + j]);
                }
            }
            grid[x_pos][y_pos].select();
        }

        void reset_position(){
            x_pos = y_pos = 0;
        }
        void update_position(int32_t dx, int32_t dy){
            // TODO: figure out if theres a nice way to do this with size_t
            grid[x_pos][y_pos].unselect();
            x_pos = std::clamp(x_pos + dx, (int32_t) 0, ((int32_t) BOARD_NROWS - 1));
            y_pos = std::clamp(y_pos + dy, (int32_t) 0, ((int32_t) BOARD_NCOLS - 1));
            std::cerr << x_pos << ' ' << y_pos << '\n';
            grid[x_pos][y_pos].select();
        }

    };
}
