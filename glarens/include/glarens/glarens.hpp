// Glarens - GUI Framework.
//
// Include all of Glarens.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#pragma once

#include "glarens/node.hpp" // IWYU pragma: keep
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

void glarens_init(SDL_Window *window, SDL_Renderer *renderer); // Initializes Glarens
void glarens_term();                                           // Terminates Glarens
