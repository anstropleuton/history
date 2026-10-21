// Glarens - GUI Framework.
//
// Glarens implementation.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#define APPDATA_GLOBAL

#include "glarens/glarens.hpp"
#include "internal/app-data.hpp"

void glarens_init(SDL_Window *window, SDL_Renderer *renderer) {
    appData.window   = window;
    appData.renderer = renderer;
}

void glarens_term() {
    appData.window   = nullptr;
    appData.renderer = nullptr;
}
