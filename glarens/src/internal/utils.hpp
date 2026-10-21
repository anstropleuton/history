// Glarens - GUI Framework.
//
// Internal application state data.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#pragma once

#include "glarens/model.hpp"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <array>
#include <cstdint>

[[nodiscard]] inline std::array<std::uint8_t, 3> hsl2rgb(float h, float s, float l) noexcept {
    auto hue2rgb = [](float p, float q, float t) noexcept -> float {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
    };

    float r, g, b;
    if (s == 0.0f) {
        r = g = b = l; // achromatic
    } else {
        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        r       = hue2rgb(p, q, h + 1.0f / 3.0f);
        g       = hue2rgb(p, q, h);
        b       = hue2rgb(p, q, h - 1.0f / 3.0f);
    }

    return {
        static_cast<std::uint8_t>(std::clamp(r * 255.0f, 0.0f, 255.0f)),
        static_cast<std::uint8_t>(std::clamp(g * 255.0f, 0.0f, 255.0f)),
        static_cast<std::uint8_t>(std::clamp(b * 255.0f, 0.0f, 255.0f))
    };
}

[[nodiscard]] inline SDL_FColor to_fcolor(SDL_Color c) noexcept {
    return SDL_FColor{
        c.r / 255.0f,
        c.g / 255.0f,
        c.b / 255.0f,
        c.a / 255.0f
    };
}

inline void draw_quad_lines(SDL_Renderer *renderer, Quad quad, SDL_Color color) noexcept {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FPoint pts[5] = {
        {quad.tl.x, quad.tl.y},
        {quad.tr.x, quad.tr.y},
        {quad.br.x, quad.br.y},
        {quad.bl.x, quad.bl.y},
        {quad.tl.x, quad.tl.y}
    };
    SDL_RenderLines(renderer, pts, 5);
}

inline void draw_quad_fill(SDL_Renderer *renderer, Quad quad, SDL_Color color) noexcept {
    SDL_FColor fcolor   = to_fcolor(color);
    SDL_Vertex verts[4] = {
        {{quad.tl.x, quad.tl.y}, fcolor, {0.0f, 0.0f}},
        {{quad.tr.x, quad.tr.y}, fcolor, {1.0f, 0.0f}},
        {{quad.br.x, quad.br.y}, fcolor, {1.0f, 1.0f}},
        {{quad.bl.x, quad.bl.y}, fcolor, {0.0f, 1.0f}}
    };
    int indices[6] = {0, 1, 2, 0, 2, 3};
    SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);
}

inline void draw_quad_texture(SDL_Renderer *renderer, SDL_Texture *texture, Quad quad, SDL_Color color) noexcept {
    SDL_FColor fcolor   = to_fcolor(color);
    SDL_Vertex verts[4] = {
        {{quad.tl.x, quad.tl.y}, fcolor, {0.0f, 0.0f}},
        {{quad.tr.x, quad.tr.y}, fcolor, {1.0f, 0.0f}},
        {{quad.br.x, quad.br.y}, fcolor, {1.0f, 1.0f}},
        {{quad.bl.x, quad.bl.y}, fcolor, {0.0f, 1.0f}}
    };
    int indices[6] = {0, 1, 2, 0, 2, 3};
    SDL_RenderGeometry(renderer, texture, verts, 4, indices, 6);
}
