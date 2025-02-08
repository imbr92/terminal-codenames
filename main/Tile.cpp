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

        if(revealed) Game::center_text(*plane, word);
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

        if(revealed) Game::center_text(*plane, word);
    }

    void Tile::set_type(Game::TileType type_){
        type = type_;
        revealed = true;
    }

}
