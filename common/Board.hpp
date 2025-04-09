#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include "Tile.hpp"
#include "GameUtils.hpp"

namespace Game {

    // Allow board to handle game state + clue drawing
    template<size_t BOARD_NROWS, size_t BOARD_NCOLS>
    class Board{
        using Grid = std::array<std::array<Tile, BOARD_NCOLS>, BOARD_NROWS>;
    private:
        int32_t x_pos, y_pos;

        // Current game board
        Grid grid;

        // Current state of game
        GameState game_state;

        // Most recent clue, num_matches = -1 if clue from old team
        Clue clue;

    public:

        Board(bool dummy = true){
            x_pos = 0, y_pos = 0;
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    const auto [x, y] = get_raw_tile_coordinates(i, j);
                    if(dummy){
                        const std::vector<std::string> words = get_words(0, BOARD_NROWS * BOARD_NCOLS);
                        const std::vector<TileType> colors = get_colors(0, BOARD_NROWS * BOARD_NCOLS);
                        grid[i][j] = Tile(i, j, words[i * BOARD_NROWS + j], colors[i * BOARD_NROWS + j], false);
                    }
                    else{
                        grid[i][j] = std::move(Tile(i, j, TILE_ROWS, TILE_COLUMNS, ""));
                    }
                }
            }
            std::cerr << "In board constructor\n";
            for(int i = 0; i < 5; ++i){
                for(int j = 0; j < 5; ++j){
                    std::cerr << (int) grid[i][j].get_type() << ' ';
                }
                std::cerr << ' ';
            }
            std::cerr << '\n';
            grid[x_pos][y_pos].select();
        }

        Board(size_t seed, bool dummy = true){
            x_pos = 0, y_pos = 0;
            const std::vector<std::string> words = get_words(seed, BOARD_NROWS * BOARD_NCOLS);
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    const auto [x, y] = get_raw_tile_coordinates(i, j);
                    if(dummy){
                        const std::vector<std::string> words = get_words(seed, BOARD_NROWS * BOARD_NCOLS);
                        const std::vector<TileType> colors = get_colors(seed, BOARD_NROWS * BOARD_NCOLS);
                        grid[i][j] = Tile(i, j, words[i * BOARD_NROWS + j], colors[i * BOARD_NROWS + j], false);
                    }
                    else{
                        grid[i][j] = std::move(Tile(i, j, TILE_ROWS, TILE_COLUMNS, words[i * BOARD_NROWS + j]));
                    }
                }
            }
            grid[x_pos][y_pos].select();
        }

        void reset_position(){
            grid[x_pos][y_pos].unselect();
            x_pos = y_pos = 0;
            grid[x_pos][y_pos].select();
        }

        void update_position(int32_t dx, int32_t dy){
            // TODO: figure out if theres a nice way to do this with size_t
            grid[x_pos][y_pos].unselect();
            x_pos = std::clamp(x_pos + dx, (int32_t) 0, ((int32_t) BOARD_NROWS - 1));
            y_pos = std::clamp(y_pos + dy, (int32_t) 0, ((int32_t) BOARD_NCOLS - 1));
            std::cerr << x_pos << ' ' << y_pos << '\n';
            grid[x_pos][y_pos].select();
        }

        void set_position(int32_t x, int32_t y){
            x_pos = x, y_pos = y;
        }

         int32_t get_x(){
            return x_pos;
        }

        int32_t get_y(){
            return y_pos;
        }

        void select(){
            grid[x_pos][y_pos].select();
        }

        const Grid& get_grid(){
            std::cerr << "got to Board::get_grid\n";
            std::cerr << "types: ";
            for(int x = 0; x < 5; ++x)
                for(int y = 0; y < 5; ++y)
                    std::cerr << (int) grid[x][y].get_type() << ' ';
            std::cerr << '\n';
            return grid;
        }

        const Tile& get_tile(size_t x_coord, size_t y_coord){
            return grid[x_coord][y_coord];
        }

        // TODO: verify that this is what we want
        void set_tile(const Tile& tile){
            std::cerr << "in set_tile\n";
            Tile& cur_tile = grid[tile.get_x()][tile.get_y()];
            std::cerr << "got cur_tile\n";
            cur_tile.set_type(tile.get_type());
            std::cerr << "set type\n";
            cur_tile.set_revealed(tile.get_revealed());
            std::cerr << "set revealed\n";
            cur_tile.set_word(tile.get_word());
            std::cerr << "set word and exiting set_tile\n";
        }

        bool get_revealed(size_t x_pos, size_t y_pos){
            return grid[x_pos][y_pos].get_revealed();
        }

        std::string get_word(size_t x_pos, size_t y_pos){
            return grid[x_pos][y_pos].get_word();
        }

        TileType get_type(size_t x_pos, size_t y_pos){
            return grid[x_pos][y_pos].get_type();
        }

        GameState get_game_state(){
            return game_state;
        }

        Clue get_clue(){
            return clue;
        }

        void set_clue(const Clue& new_clue){
            clue = new_clue;
        }

        void set_revealed(size_t x_pos, size_t y_pos, bool revealed_){
            grid[x_pos][y_pos].set_revealed(revealed_);
        }

        void set_type(size_t x_pos, size_t y_pos, TileType type_){
            grid[x_pos][y_pos].set_type(type_);
        }

        void set_word(size_t x_pos, size_t y_pos, const std::string& word_){
            grid[x_pos][y_pos].set_word(word_);
        }

        void draw(size_t x_pos, size_t y_pos){
            grid[x_pos][y_pos].draw();
        }

        void draw(){
            for(size_t i = 0; i < BOARD_NCOLS; ++i){
                for(size_t j = 0; j < BOARD_NROWS; ++j){
                    grid[i][j].draw();
                }
            }
        }


        void set_winner(Team winner){}

        void update_game_state(const GameState &new_game_state){
            game_state = new_game_state;
        }

        // Return true iff guesses is now 0
        bool decrement_guesses(){
            return --clue.num_matches == 0;
        }

        // TODO: Remove eventually
        void color_test(){
            grid[0][0].set_type(Game::TileType::RED);
            grid[0][1].set_type(Game::TileType::BLUE);
            grid[0][2].set_type(Game::TileType::YELLOW);
            grid[0][3].set_type(Game::TileType::BLACK);
            grid[0][4].set_type(Game::TileType::UNKNOWN);
        }

    };
}
