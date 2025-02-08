#pragma once

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "../common/Board.hpp"

namespace Game {

    template<size_t BOARD_NROWS, size_t BOARD_NCOLS>
    class Server{

    using Grid = std::array<std::array<Tile, BOARD_NCOLS>, BOARD_NROWS>;

    private:
        Board<BOARD_NROWS, BOARD_NCOLS> game_state;

    public:
        // Returns grid with hidden words
        Grid get_full_grid(){
            return game_state.get_grid();
        }

        // Returns grid with only revealed cells filled out
        Grid get_partial_grid(){
            Grid hidden_grid;
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    if(game_state.is_revealed(i, j)){
                        if(game_state.is_revealed(i, j)){
                            hidden_grid[i][j] = Tile(i, j, game_state.get_word(i, j), game_state.get_type(i, j), true);
                        }
                        else{
                            hidden_grid[i][j] = Tile(i, j, "", TileType::UNKNOWN, false);
                        }
                    }
                }
            }
            return hidden_grid;
        }

        // TODO: Support the following:

        // Send board
        // Receive clue
        // Send clue
        // Handle connections

    };

}
