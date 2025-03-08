#pragma once

#include <string>
#include <memory>
#include <optional>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "GameUtils.hpp"

namespace Game {

    class Tile {

    private:
        bool revealed, dummy;
        TileType type;
        size_t x, y;
        std::string word;
        std::optional<std::unique_ptr<ncpp::Plane>> plane;

    public:
        Tile() = default;

        // For dummy tiles --> no plane, used on server side + to pass to client
        Tile(size_t x_pos, size_t y_pos, const std::string& word_, TileType type_, bool revealed_);

        // For real tiles (with planes), used on client side
        Tile(size_t x_pos, size_t y_pos, size_t height, size_t width, const std::string& word_);

        Tile(const Tile& other) noexcept;
        Tile(Tile&& other) noexcept;
        Tile& operator=(const Tile& other) noexcept;
        Tile& operator=(Tile&& other) noexcept;

        void select();
        void unselect();
        std::string get_word() const;
        size_t get_x() const;
        size_t get_y() const;
        TileType get_type() const;
        bool get_revealed() const;
        void draw();
        void set_revealed(bool revealed_);
        void set_type(Game::TileType type_);
        void set_word(const std::string& word_);

    };

}
