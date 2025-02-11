#pragma once

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "../common/Board.hpp"

namespace Game {


    template<size_t BOARD_NROWS, size_t BOARD_NCOLS>
    class Client{

    using Grid = std::array<std::array<Tile, BOARD_NCOLS>, BOARD_NROWS>;

    private:
        Team team;
        Board<BOARD_NROWS, BOARD_NCOLS> game_state;

    public:
        // TODO: Support the following:
        // Update game state (with dummy board) -- DONE
        // Receive board (with dummy tiles)
        // Receive Clue (Both clue receiver and sender to receive opponent clues)
        // Send Clue (Only sender)
        // Send Selection (Only receiver)

        // Updates board in a way such that it is pushed to ncpp
        void update_board(const Grid& dummy_board){
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    game_state.set_revealed(i, j, dummy_board[i][j].is_revealed());
                    game_state.set_type(i, j, dummy_board[i][j].get_type());
                    game_state.set_word(i, j, dummy_board[i][j].get_word());
                }
            }
            game_state.draw();
        }
        // Updates full game state in a way such that it is pushed to ncpp
        void update_state(Selection s){
            
        }

    };

}
