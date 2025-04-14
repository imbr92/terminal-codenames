#include <iostream>
#include <array>

#include "Tile.hpp"
#include "GameUtils.hpp"

namespace Game {

    Tile::Tile(): dummy(true) {}

    Tile::Tile(size_t x_pos, size_t y_pos, size_t height, size_t width, const std::string& word_){
        word = word_;
        x = x_pos;
        y = y_pos;
        revealed = false;
        type = TileType::UNKNOWN;
        const auto [x, y] = get_raw_tile_coordinates(x_pos, y_pos);
        plane = std::make_unique<ncpp::Plane>(height, width, x, y);
        dummy = false;
        Tile::unselect();
    }

    Tile::Tile(size_t x_pos, size_t y_pos, const std::string& word_, TileType type_, bool revealed_){
        word = word_;
        x = x_pos;
        y = y_pos;
        revealed = revealed_;
        type = type_;
        plane = {};
        dummy = true;
    }

    Tile::Tile(const Tile& other) noexcept{
        if(this != &other){
            revealed = other.revealed;
            type = other.type;
            x = other.x;
            y = other.y;
            word = other.word;
            dummy = true;
        }
    }

    Tile& Tile::operator=(const Tile& other) noexcept{
        if(this != &other){
            revealed = other.revealed;
            type = other.type;
            x = other.x;
            y = other.y;
            word = other.word;
            dummy = true;
        }
        return *this;
    }

    Tile::Tile(Tile&& other) noexcept{
        revealed = other.revealed;
        type = other.type;
        x = other.x;
        y = other.y;
        dummy = other.dummy;
        word = std::move(other.word);
        plane = std::move(other.plane);
    }

    Tile& Tile::operator=(Tile&& other) noexcept{
        if(this != &other){
            revealed = other.revealed;
            type = other.type;
            x = other.x;
            y = other.y;
            dummy = other.dummy;
            word = std::move(other.word);
            plane = std::move(other.plane);
        }
        return *this;
    }


    void Tile::unselect(){
        if(dummy) return;
        if(type == TileType::UNKNOWN){
            plane.value()->set_base("", 0, Game::DEFAULT_UNSELECTED);
        } else if(type == TileType::RED){
            plane.value()->set_base("", 0, Game::RED_UNSELECTED);
        } else if(type == TileType::BLUE){
            plane.value()->set_base("", 0, Game::BLUE_UNSELECTED);
        } else if(type == TileType::YELLOW){
            plane.value()->set_base("", 0, Game::YELLOW_UNSELECTED);
        } else if(type == TileType::BLACK){
            plane.value()->set_base("", 0, Game::BLACK_UNSELECTED);
        }

        // TODO: Check if revealed refers to whether the color was revealed or the word
        if(revealed) draw();
    }

    void Tile::select(){
        if(dummy) return;
        if(type == TileType::UNKNOWN){
            plane.value()->set_base("", 0, Game::DEFAULT_SELECTED);
        } else if(type == TileType::RED){
            plane.value()->set_base("", 0, Game::RED_SELECTED);
        } else if(type == TileType::BLUE){
            plane.value()->set_base("", 0, Game::BLUE_SELECTED);
        } else if(type == TileType::YELLOW){
            plane.value()->set_base("", 0, Game::YELLOW_SELECTED);
        } else if(type == TileType::BLACK){
            plane.value()->set_base("", 0, Game::BLACK_SELECTED);
        }

        if(revealed) draw();
    }

    void Tile::draw(){
        Game::center_text(*plane.value(), word);
    }

    void Tile::set_type(Game::TileType type_){
        type = type_;
        if(type != Game::TileType::UNKNOWN){
            revealed = true;
        }
    }

    void Tile::set_revealed(bool revealed_){
        revealed = revealed_;
    }

    void Tile::set_word(const std::string& word_){
        word = word_;
    }

    const std::string& Tile::get_word() const{
        return word;
    }

    size_t Tile::get_x() const{
        return x;
    }

    size_t Tile::get_y() const{
        return y;
    }

    TileType Tile::get_type() const{
        return type;
    }

    bool Tile::get_revealed() const{
        return revealed;
    }

    bool Tile::get_dummy() const{
        return dummy;
    }

}
