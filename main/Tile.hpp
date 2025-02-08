#pragma once

#include <string>
#include <memory>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

namespace Game {

    class Tile {

    private:
        size_t x, y;
        std::string word;
        std::unique_ptr<ncpp::Plane> plane;

    public:
        Tile() = default;

        Tile(size_t x_pos, size_t y_pos, size_t height, size_t width, const std::string& word_);

        void select();
        void unselect();

    };

}
