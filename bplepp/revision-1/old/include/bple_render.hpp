

#ifndef BPLE_RENDER_HPP
#define BPLE_RENDER_HPP

#include <vector>

#include "bple_data.hpp"
#include "raylib.h"

#ifndef QUALIFIER
#define QUALIFIER extern
#endif

#ifndef TEXTURES_PATH
#define TEXTURES_PATH "res/textures"
#endif

struct Resources {
    // Part textures for [type][skin][prop] as subscripts
    std::vector<std::vector<std::vector<Texture>>> part_textures;

    void load()
    {
    }

    void unload()
    {
    }
} QUALIFIER resources;

#endif // BPLE_RENDER_HPP
