#ifndef SPRITES_HPP
#define SPRITES_HPP

#include <string>

struct sprite_data_t {
    std::string name;

    // 0 -> IngameAtlas.png
    // 1 -> IngameAtlas2.png
    // 2 -> IngameAtlas3.png
    // 3 -> MenuAtlas.png
    // 4 -> MenuAtlas2.png
    int texture_id;
    float x;
    float y;
    float width;
    float height;
};

#endif // SPRITES_HPP
