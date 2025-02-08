#include "Tile.hpp"
#include "GameUtils.hpp"

namespace Game {

    Tile::Tile(size_t x_pos, size_t y_pos, size_t height, size_t width, const std::string& word_){
            word = word_;
            x = x_pos;
            y = y_pos;
            plane = std::make_unique<ncpp::Plane>(height, width, x_pos, y_pos);
            Tile::unselect();
    }

    void Tile::unselect(){
        // TODO: move these colors to namespace level constants
        plane->set_fg_rgb8(0xc0, 0x80, 0xc0);
        plane->set_bg_rgb8(0x20, 0x00, 0x20);
        plane->set_base("", 0, NCCHANNELS_INITIALIZER(0xc0, 0x80, 0xc0, 0x20, 0, 0x20));

        Game::center_text(*plane, word);
    }

    void Tile::select(){
        // TODO: move these colors to namespace level constants
        plane->set_fg_rgb8(0xFF, 0xA0, 0xFF);  // Light Pinkish-Purple
        plane->set_bg_rgb8(0x60, 0x30, 0x60);  // Darker Purple
        plane->set_base("", 0, NCCHANNELS_INITIALIZER(0xFF, 0xFF, 0xFF, 0x80, 0x30, 0x80));

        Game::center_text(*plane, word);
    }

}
