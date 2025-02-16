#include "Tile.hpp"
#include "GameUtils.hpp"

namespace Game {

    Tile::Tile(size_t x_pos, size_t y_pos, size_t height, size_t width, const std::string& word_){
        word = word_;
        x = x_pos;
        y = y_pos;
        revealed = false;
        type = TileType::UNKNOWN;
        plane = std::make_unique<ncpp::Plane>(height, width, x_pos, y_pos);
        Tile::unselect();
    }

    Tile::Tile(size_t x_pos, size_t y_pos, const std::string& word_, TileType type_, bool revealed_){
        word = word_;
        x = x_pos;
        y = y_pos;
        revealed = revealed_;
        type = type_;
        plane = nullptr;
    }

    void Tile::unselect(){
        if(type == TileType::UNKNOWN){
            plane->set_base("", 0, Game::DEFAULT_UNSELECTED);
        } else if(type == TileType::RED){
            plane->set_base("", 0, Game::RED_UNSELECTED);
        } else if(type == TileType::BLUE){
            plane->set_base("", 0, Game::BLUE_UNSELECTED);
        } else if(type == TileType::YELLOW){
            plane->set_base("", 0, Game::YELLOW_UNSELECTED);
        } else if(type == TileType::BLACK){
            plane->set_base("", 0, Game::BLACK_UNSELECTED);
        }

        if(revealed) draw();
    }

    void Tile::select(){
        if(type == TileType::UNKNOWN){
            plane->set_base("", 0, Game::DEFAULT_SELECTED);
        } else if(type == TileType::RED){
            plane->set_base("", 0, Game::RED_SELECTED);
        } else if(type == TileType::BLUE){
            plane->set_base("", 0, Game::BLUE_SELECTED);
        } else if(type == TileType::YELLOW){
            plane->set_base("", 0, Game::YELLOW_SELECTED);
        } else if(type == TileType::BLACK){
            plane->set_base("", 0, Game::BLACK_SELECTED);
        }

        if(revealed) draw();
    }

    void Tile::draw(){
        Game::center_text(*plane, word);
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


    bool Tile::is_revealed(){
        return revealed;
    }

    std::string Tile::get_word() const{
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

}
