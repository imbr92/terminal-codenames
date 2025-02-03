#include "Tile.hpp"
#include "GameUtils.hpp"

namespace Game {

    Tile::Tile(size_t x_pos, size_t y_pos, size_t height, size_t width, const std::string& word_){
        word = word_;
            x = x_pos;
            y = y_pos;
            plane = std::make_unique<ncpp::Plane>(height, width, x_pos, y_pos);
            // TODO: move these colors to namespace level constants
            plane->set_fg_rgb8(0xc0, 0x80, 0xc0);
            plane->set_bg_rgb8(0x20, 0x00, 0x20);
            plane->set_base("", 0, NCCHANNELS_INITIALIZER(0xc0, 0x80, 0xc0, 0x20, 0, 0x20));
            Game::center_text(*plane, word_);
    }

}
