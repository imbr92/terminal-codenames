#pragma once

#include <string>
#include <memory>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "GameUtils.hpp"

namespace Game {

    class Tile {

    private:
        bool revealed;
        TileType type;
        size_t x, y;
        std::string word;
        std::unique_ptr<ncpp::Plane> plane;

    public:
        Tile() = default;

        // For dummy tiles
        Tile(size_t x_pos, size_t y_pos, const std::string& word_, TileType type_, bool revealed_);

        Tile(size_t x_pos, size_t y_pos, size_t height, size_t width, const std::string& word_);

        void select();
        void unselect();
        std::string get_word() const;
        size_t get_x() const;
        size_t get_y() const;
        TileType get_type() const;
        bool is_revealed();
        void draw();
        void set_revealed(bool revealed_);
        void set_type(Game::TileType type_);
        void set_word(const std::string& word_);

    };

}
