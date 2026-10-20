#ifndef DATA_HPP
#define DATA_HPP

#include <vector>

#include "parts.hpp"
#include "sprites.hpp"

extern std::vector<base_part_t> parts;
void init_parts();

extern std::vector<sprite_data_t> sprites;
void init_sprites();

#endif // DATA_HPP
