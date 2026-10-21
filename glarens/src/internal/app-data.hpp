// Glarens - GUI Framework.
//
// Internal application state data.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#pragma once

#include "glarens/node.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <memory>

#ifndef APPDATA_GLOBAL
#define APPDATA_GLOBAL extern
#endif

APPDATA_GLOBAL struct AppData {
    SDL_Window           *window;
    SDL_Renderer         *renderer;
    std::shared_ptr<Node> root;
} appData;
