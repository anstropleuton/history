/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Minesweeper-specific GUI stuff.
 *
 *  @copyright  Copyright (c) 2024 Anstro Pleuton
 *
 *   __  __ _             _
 *  |  \/  (_)_ __  _   _(_)
 *  | |\/| | | '_ \| | | | |
 *  | |  | | | | | | |_| | |
 *  |_|  |_|_|_| |_|\__,_|_|
 *
 *  Minui is a GUI framework for Minesweeper.
 *
 *  This software is licensed under the terms of MIT License.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  Credits where credit's due:
 *  - ASCII Art generated using https://www.patorjk.com/software/taag with font
 *    "Standard".
 *  - Textures taken from Leyan Lo,
 *    https://github.com/leyanlo/minesweeper/tree/main/src/images
 *  - Windows 95 style pixel font taken from Alina Sava,
 *    https://fontsarena.com/w95fa-by-alina-sava
 */

#pragma once

#include "minui.hpp"
#include "raylib.h"
#include "raylib_shaders.h"

#ifndef MINUI_MS_EXTERN
/**
 *  @brief  Define this to define global variables.
 */
#define MINUI_MS_EXTERN extern
#endif // ifndef MINUI_MS_EXTERN

/**
 *  @brief  All Minui's contents in this namespace.
 */
namespace minui_ms {

// Theme colors

/**
 *  @brief  Light theme: Highlight color
 */
const Color lt_highlight = { 255, 255, 255, 255 };

/**
 *  @brief  Light theme: Face color
 */
const Color lt_face = { 191, 191, 191, 255 };

/**
 *  @brief  Light theme: Shadow color
 */
const Color lt_shadow = { 127, 127, 127, 255 };

/**
 *  @brief  Light theme: Highlight color when hovered
 */
const Color lt_hover_highlight = { 255, 255, 255, 255 };

/**
 *  @brief  Light theme: Face color when hovered
 */
const Color lt_hover_face = { 207, 207, 207, 255 };

/**
 *  @brief  Light theme: Shadow color when hovered
 */
const Color lt_hover_shadow = { 143, 143, 143, 255 };

/**
 *  @brief  Light theme: Highlight color when held
 */
const Color lt_held_highlight = { 239, 239, 239, 255 };

/**
 *  @brief  Light theme: Face color when held
 */
const Color lt_held_face = { 175, 175, 175, 255 };

/**
 *  @brief  Light theme: Shadow color when held
 */
const Color lt_held_shadow = { 111, 111, 111, 255 };

/**
 *  @brief  Dark theme: Highlight color
 */
const Color dt_highlight = { 127, 127, 127, 255 };

/**
 *  @brief  Dark theme: Face color
 */
const Color dt_face = { 63, 63, 63, 255 };

/**
 *  @brief  Dark theme: Shadow color
 */
const Color dt_shadow = { 0, 0, 0, 255 };

/**
 *  @brief  Dark theme: Highlight color when hovered
 */
const Color dt_hover_highlight = { 143, 143, 143, 255 };

/**
 *  @brief  Dark theme: Face color when hovered
 */
const Color dt_hover_face = { 79, 79, 79, 255 };

/**
 *  @brief  Dark theme: Shadow color when hovered
 */
const Color dt_hover_shadow = { 16, 16, 16, 255 };

/**
 *  @brief  Dark theme: Highlight color when held
 */
const Color dt_held_highlight = { 111, 111, 111, 255 };

/**
 *  @brief  Dark theme: Face color when held
 */
const Color dt_held_face = { 47, 47, 47, 255 };

/**
 *  @brief  Dark theme: Shadow color when held
 */
const Color dt_held_shadow = { 0, 0, 0, 255 };

// Textures taken from Leyan Lo,
// https://github.com/leyanlo/minesweeper/tree/main/src/images

/**
 *  @brief  7-segment display textures
 *
 *  0-9 numbers (indices 0-9) and then a '-' (index 10).  11 items total.
 */
MINUI_MS_EXTERN std::array<Texture, 11> sd7_textures;

/**
 *  @brief  Cell number texture.
 *
 *  1-8 numbers (indices 0-7).  8 items total.
 */
MINUI_MS_EXTERN std::array<Texture, 8> cell_textures;

/**
 *  @brief  Flag texture.
 */
MINUI_MS_EXTERN Texture flag_texture;

/**
 *  @brief  Question-mark texture.
 */
MINUI_MS_EXTERN Texture q_mark_texture;

/**
 *  @brief  Mine texture.
 */
MINUI_MS_EXTERN Texture mine_texture;

/**
 *  @brief  Face texture.
 */
MINUI_MS_EXTERN Texture face_texture;

/**
 *  @brief  Clicking face texture.
 */
MINUI_MS_EXTERN Texture clicking_face_texture;

/**
 *  @brief  Won face texture.  8)
 */
MINUI_MS_EXTERN Texture won_face_texture;

/**
 *  @brief  Lost face texture.  X(
 */
MINUI_MS_EXTERN Texture lost_face_texture;

/**
 *  @brief  Windows 95 style pixel font.
 */
MINUI_MS_EXTERN Font windows_95_font;

/**
 *  @brief  Load all the resources.
 *  @note   Also sets every extern global variables, including those defined
 *          after this function.
 */
auto load_minui() -> void;

/**
 *  @brief  Unload all the resources.
 */
auto unload_minui() -> void;

/**
 *  @brief  Rounded rectangle shader.
 */
MINUI_MS_EXTERN Shader rounded_rectangle_shader;

/**
 *  @brief  Rounded rectangle layout.
 */
struct rounded_rectangle : minui::layout {

    // Shader parameters

    /**
     *  @brief  Configured rounded rectangle shader parameters.
     */
    RoundedRectangle config = {
        .cornerRadius       = { 20.0f, 20.0f, 20.0f, 20.0f },
        .shadowRadius       = 20.0f,
        .shadowOffset       = { 0.0f, -5.0f },
        .shadowScale        = 0.95f,
        .borderThickness    = 1.0f,
    };

    // Extra config
    Color rectangleColor = GRAY;
    Color shadowColor    = ColorAlpha(BLACK, 0.25f);
    Color borderColor    = ColorAlpha(WHITE, 0.125f);

    /**
     *  @brief  Default constructor.
     */
    rounded_rectangle() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~rounded_rectangle() = default;

    /**
     *  @brief  Layout updates before children.
     */
    virtual auto pre_update() -> void override;

    /**
     *  @brief  Layout updates after children.
     */
    virtual auto post_update() -> void override
    {}

    /**
     *  @brief  Layout draws before children.
     */
    virtual auto pre_draw() const -> void override;

    /**
     *  @brief  Layout draws after children.
     */
    virtual auto post_draw() const -> void override
    {}
};

/**
 *  @brief  Pixel scale is used to give pixelated.  Pixel scale of 1 is no
 *          pixel scaling.  I find pixel scale of 4 to be quite neat.
 */
MINUI_MS_EXTERN float pixel_scale;

/**
 *  @brief  Draw Minesweeper-styled beveled rectangle.
 *
 *  @param  rec           Rectangle.
 *  @param  border_thick  Border thickness (in scaled pixel).
 */
auto draw_beveled_rectangle(
    Rectangle rec,
    int       border_thick,
    Color     center_color,
    Color     top_color,
    Color     bottom_color,
    Color     left_color,
    Color     right_color,
    Color     top_left_color,
    Color     top_right_color,
    Color     bottom_left_color,
    Color     bottom_right_color
) -> void;

/**
 *  @brief  Helper to draw beveled rectangle with 3 colors.
 *
 *  @param  rec                 Rectangle.
 *  @param  border_thick        Border thickness.
 *  @param  center_color        Center and top-right and bottom-left corners.
 *  @param  top_left_color      Top-left edge and corner.
 *  @param  bottom_right_color  Bottom-right edge and corner.
 */
auto draw_beveled_rectangle(
    Rectangle rec,
    int       border_thick,
    Color     center_color,
    Color     top_left_color,
    Color     bottom_right_color
) -> void;

}
