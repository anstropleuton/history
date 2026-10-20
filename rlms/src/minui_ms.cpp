/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Implementations for non-inline functions from
 *           @c minui_ms.hpp .
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

#define MINUI_MS_EXTERN
#include "minui_ms.hpp"

/**
 *  @brief  Load all the resources.
 */
auto minui_ms::load_minui() -> void
{
    flag_texture          = LoadTexture("resources/flag.png");
    q_mark_texture        = LoadTexture("resources/q_mark.png");
    mine_texture          = LoadTexture("resources/mine.png");
    face_texture          = LoadTexture("resources/face.png");
    clicking_face_texture = LoadTexture("resources/clicking_face.png");
    won_face_texture      = LoadTexture("resources/won_face.png");
    lost_face_texture     = LoadTexture("resources/lost_face.png");

    for (int i = 0; i < sd7_textures.size(); i++)
    {
        char c          = "0123456789-"[i];
        sd7_textures[i] = LoadTexture(
            std::format("resources/sd7_{}.png", c).c_str()
        );
    }

    for (int i = 0; i < cell_textures.size(); i++)
    {
        char c           = "12345678"[i];
        cell_textures[i] = LoadTexture(
            std::format("resources/cell_{}.png", c).c_str()
        );
    }

    windows_95_font = LoadFont("resources/windows_95.otf");

    pixel_scale = 4;
}

/**
 *  @brief  Unload all the resources.
 */
auto minui_ms::unload_minui() -> void
{
    UnloadTexture(flag_texture);
    UnloadTexture(q_mark_texture);
    UnloadTexture(mine_texture);
    UnloadTexture(face_texture);
    UnloadTexture(clicking_face_texture);
    UnloadTexture(won_face_texture);
    UnloadTexture(lost_face_texture);

    for (auto &texture : sd7_textures)
    {
        UnloadTexture(texture);
    }

    for (auto &texture : cell_textures)
    {
        UnloadTexture(texture);
    }

    UnloadFont(windows_95_font);
}

/**
 *  @brief  Draw Minesweeper-styled beveled rectangle.
 *
 *  @param  rec           Rectangle.
 *  @param  border_thick  Border thickness.
 */
auto minui_ms::draw_beveled_rectangle(
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
) -> void
{
    // Don't ask questions
    // uncrustify:off

    // Draw borders
    for (int i = 0; i < border_thick; i++)
    {
        // Edges
        DrawRectangleRec(Rectangle { rec.x              + (i + 1) * pixel_scale, rec.y              + (i + 0) * pixel_scale, rec.width  - (i + 1) * 2 * pixel_scale, pixel_scale }, top_color);
        DrawRectangleRec(Rectangle { rec.x              + (i + 1) * pixel_scale, rec.y + rec.height - (i + 1) * pixel_scale, rec.width  - (i + 1) * 2 * pixel_scale, pixel_scale }, bottom_color);
        DrawRectangleRec(Rectangle { rec.x              + (i + 0) * pixel_scale, rec.y              + (i + 1) * pixel_scale, pixel_scale, rec.height - (i + 1) * 2 * pixel_scale }, left_color);
        DrawRectangleRec(Rectangle { rec.x + rec.width  - (i + 1) * pixel_scale, rec.y              + (i + 1) * pixel_scale, pixel_scale, rec.height - (i + 1) * 2 * pixel_scale }, right_color);

        // Corners
        DrawRectangleRec(Rectangle { rec.x              + (i + 0) * pixel_scale, rec.y              + (i + 0) * pixel_scale, pixel_scale, pixel_scale }, top_left_color);
        DrawRectangleRec(Rectangle { rec.x + rec.width  - (i + 1) * pixel_scale, rec.y              + (i + 0) * pixel_scale, pixel_scale, pixel_scale }, top_right_color);
        DrawRectangleRec(Rectangle { rec.x              + (i + 0) * pixel_scale, rec.y + rec.height - (i + 1) * pixel_scale, pixel_scale, pixel_scale }, bottom_left_color);
        DrawRectangleRec(Rectangle { rec.x + rec.width  - (i + 1) * pixel_scale, rec.y + rec.height - (i + 1) * pixel_scale, pixel_scale, pixel_scale }, bottom_right_color);
    }

    // Center
    DrawRectangleRec(Rectangle { rec.x + border_thick * pixel_scale, rec.y + border_thick * pixel_scale, rec.width - border_thick * 2 * pixel_scale, rec.height - border_thick * 2 * pixel_scale }, center_color);

    // uncrustify:on
}

/**
 *  @brief  Helper to draw beveled rectangle with 3 colors.
 *
 *  @param  rec                 Rectangle.
 *  @param  border_thick        Border thickness.
 *  @param  center_color        Center and top-right and bottom-left corners.
 *  @param  top_left_color      Top-left edge and corner.
 *  @param  bottom_right_color  Bottom-right edge and corner.
 */
auto minui_ms::draw_beveled_rectangle(
    Rectangle rec,
    int       border_thick,
    Color     center_color,
    Color     top_left_color,
    Color     bottom_right_color
) -> void
{
    draw_beveled_rectangle(rec, border_thick, center_color,
        top_left_color, bottom_right_color, top_left_color, bottom_right_color,
        top_left_color, center_color, center_color, bottom_right_color);
}

/**
 *  @brief  Layout updates before children.
 */
auto minui_ms::rounded_rectangle::pre_update() -> void
{
    if (rounded_rectangle_shader.id == 0)
    {
        rounded_rectangle_shader = LoadShader(NULL,
            "res/glsl330/rounded_rectangle.fs");
    }

    if (rounded_rectangle_shader.id == 0)
    {
        throw std::runtime_error("Unable to load rounded rectangle shader");
    }

    if (!config.configured)
    {
        config = CreateRoundedRectangle(config.cornerRadius,
            config.shadowRadius, config.shadowOffset, config.shadowScale,
            config.borderThickness, rounded_rectangle_shader);
    }
}

/**
 *  @brief  Layout draws before children.
 */
auto minui_ms::rounded_rectangle::pre_draw() const -> void
{
    auto rec = drawable_bounds();

    UpdateRoundedRectangle(config, rounded_rectangle_shader);

    // uncrustify:off
    // Flip Y axis to match shader coordinate system
    rec.y = GetScreenHeight() - rec.y - rec.height;
    SetShaderValue(rounded_rectangle_shader, config.rectangleLoc, (float[]){ rec.x, rec.y, rec.width, rec.height }, SHADER_UNIFORM_VEC4);
    SetShaderValue(rounded_rectangle_shader, config.colorLoc, (float[]) { rectangleColor.r/255.0f, rectangleColor.g/255.0f, rectangleColor.b/255.0f, rectangleColor.a/255.0f }, SHADER_UNIFORM_VEC4);
    SetShaderValue(rounded_rectangle_shader, config.shadowColorLoc, (float[]) { shadowColor.r/255.0f, shadowColor.g/255.0f, shadowColor.b/255.0f, shadowColor.a/255.0f }, SHADER_UNIFORM_VEC4);
    SetShaderValue(rounded_rectangle_shader, config.borderColorLoc, (float[]) { borderColor.r/255.0f, borderColor.g/255.0f, borderColor.b/255.0f, borderColor.a/255.0f }, SHADER_UNIFORM_VEC4);
    // uncrustify:on

    BeginShaderMode(rounded_rectangle_shader);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    EndShaderMode();

    draw_debug_bounds();
}
