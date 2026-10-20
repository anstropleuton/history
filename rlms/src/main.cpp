/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Minesweeper game.
 *
 *  @copyright  Copyright (c) 2024 Anstro Pleuton
 *
 *   __  __ _
 *  |  \/  (_)_ __   ___  _____      _____  ___ _ __   ___ _ __
 *  | |\/| | | '_ \ / _ \/ __\ \ /\ / / _ \/ _ \ '_ \ / _ \ '__|
 *  | |  | | | | | |  __/\__ \\ V  V /  __/  __/ |_) |  __/ |
 *  |_|  |_|_|_| |_|\___||___/ \_/\_/ \___|\___| .__/ \___|_|
 *                                             |_|
 *
 *  This project implements the game Minesweeper along with additional features
 *  to enhance the game.
 *
 *  The features include:
 *  - Core Minesweeper game.
 *  - Generate mines after the first click.
 *  - Always generate completely logically solvable board. This means that the
 *    player need have to guess or take risks.
 *  - Flag: Flag prevents accidental left click to reveal the cell when the cell
 *    is flagged.
 *  - Question Mark: Question marking achieves the effect of flag, but does not
 *    count in the Mines Count display.
 *  - Speed reveal: Left clicking a revealed cell with mine number > 0, if the
 *    number of adjacent hidden cells without a flag equal to the number on the
 *    clicked cell, reveals all the adjacent hidden cells without a flag.
 *  - Speed flag: Right clicking a revealed cell with mine number > 0, if the
 *    number of adjacent hidden cells equal to the number on the clicked cell,
 *    flags all the adjacent hidden cells.
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
 */

#include <memory>

#define RAYLIB_SHADERS_IMPLEMENTATION

#include "minui.hpp"
#include "minui_ms.hpp"
#include "raylib.h"

using namespace std::string_literals;

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 500, "Minui Test");
    SetTargetFPS(60);

    minui_ms::load_minui();
    flux::add_builtin_functions();
    flux::add_builtin_constants();

    minui::draw_debug = true;

    auto root = minui::make_layout<minui::screen_layout>();

    auto div_row = minui::make_layout<minui::row_layout>(root);

    auto div_col_1 = minui::make_layout<minui::linear_item>(div_row);
    auto div_col_2 = minui::make_layout<minui::linear_item>(div_row);

    div_col_1->rule.min_shrink = { "psx" };
    div_col_1->rule.weight     = 1.0f;
    div_col_2->rule.min_shrink = { "psx" };
    div_col_2->rule.weight     = 4.0f;

    auto col_box_1 = minui::make_layout<minui::column_layout>(div_col_1);
    auto col_box_2 = minui::make_layout<minui::column_layout>(div_col_2);

    auto item_1 = minui::make_layout<minui::linear_item>(col_box_1);
    auto item_2 = minui::make_layout<minui::linear_item>(col_box_1);
    auto item_3 = minui::make_layout<minui::linear_item>(col_box_2);
    auto item_4 = minui::make_layout<minui::linear_item>(col_box_2);

    item_1->rule.min_shrink = { "psy" };
    item_1->rule.weight     = 1.0f;
    item_2->rule.min_shrink = { "psy" };
    item_2->rule.weight     = 4.0f;
    item_3->rule.min_shrink = { "psy" };
    item_3->rule.weight     = 1.0f;
    item_4->rule.min_shrink = { "psy" };
    item_4->rule.weight     = 4.0f;

    auto box_1 = minui::make_layout<minui::box_layout>(item_1);
    auto box_2 = minui::make_layout<minui::box_layout>(item_2);
    auto box_3 = minui::make_layout<minui::box_layout>(item_3);
    auto box_4 = minui::make_layout<minui::box_layout>(item_4);

    box_1->rule.shrink = { "20" };
    box_2->rule.shrink = { "20" };
    box_3->rule.shrink = { "20" };
    box_4->rule.shrink = { "20" };

    auto rr_1 = minui::make_layout<minui_ms::rounded_rectangle>(box_1);
    auto rr_2 = minui::make_layout<minui_ms::rounded_rectangle>(box_2);
    auto rr_3 = minui::make_layout<minui_ms::rounded_rectangle>(box_3);
    auto rr_4 = minui::make_layout<minui_ms::rounded_rectangle>(box_4);

    rr_1->rectangleColor = { 48, 48, 48, 255 };
    rr_2->rectangleColor = { 48, 48, 48, 255 };
    rr_3->rectangleColor = { 48, 48, 48, 255 };
    rr_4->rectangleColor = { 48, 48, 48, 255 };

    auto main_row = minui::make_layout<minui::row_layout>(rr_3);
    auto main_col = minui::make_layout<minui::column_layout>(rr_2);

    for (std::size_t i = 0; i < 3; i++)
    {
        auto spacer = minui::make_layout<minui::linear_item>(main_row);
        spacer->rule.min_shrink = { "psx - 10" };

        auto item = minui::make_layout<minui::linear_item>(main_row);
        item->rule.min_shrink = { "psx - 50" };
        item->rule.max_shrink = { "psx - 200" };
        item->rule.weight     = 1.0f;

        auto box         = minui::make_layout<minui::box_layout>(item);
        box->rule.shrink = { "", "20" };

        auto rr = minui::make_layout<minui_ms::rounded_rectangle>(box);

        rr->rectangleColor      = { 80, 80, 80, 255 };
        rr->shadowColor         = { 0, 0, 0, 0 };
        rr->config.cornerRadius = { 10.0f, 10.0f, 10.0f, 10.0f };

        if (i == 2)
        {
            auto spacer = minui::make_layout<minui::linear_item>(main_row);
            spacer->rule.min_shrink = { "psx - 10" };
        }
    }

    auto main_row_end_spacer = minui::make_layout<minui::linear_item>(main_row);
    main_row_end_spacer->rule.min_shrink = { "psx" };
    main_row_end_spacer->rule.weight     = 0.001f;

    for (std::size_t i = 0; i < 9; i++)
    {
        auto spacer = minui::make_layout<minui::linear_item>(main_col);
        spacer->rule.min_shrink = { "psy - 10" };

        auto item = minui::make_layout<minui::linear_item>(main_col);
        item->rule.min_shrink = { "psy - 20" };
        item->rule.max_shrink = { "psy - 40" };
        item->rule.weight     = 1.0f;

        auto box         = minui::make_layout<minui::box_layout>(item);
        box->rule.shrink = { "20", "" };

        auto rr = minui::make_layout<minui_ms::rounded_rectangle>(box);

        rr->rectangleColor      = { 80, 80, 80, 255 };
        rr->shadowColor         = { 0, 0, 0, 0 };
        rr->config.cornerRadius = { 10.0f, 10.0f, 10.0f, 10.0f };

        if (i == 8)
        {
            auto spacer = minui::make_layout<minui::linear_item>(main_col);
            spacer->rule.min_shrink = { "psy - 10" };
        }
    }

    auto main_col_end_spacer = minui::make_layout<minui::linear_item>(main_col);
    main_col_end_spacer->rule.min_shrink = { "psy" };
    main_col_end_spacer->rule.weight     = 0.001f;

    auto grid = minui::make_layout<minui::grid_layout>(rr_4);

    grid->rows.resize(4);
    for (auto &row : grid->rows)
    {
        row.min_shrink = { "psy - 20" };
        row.weight     = 1.0f;
    }

    grid->columns.resize(3);
    for (auto &col : grid->columns)
    {
        col.min_shrink = { "psx - 20" };
        col.weight     = 1.0f;
    }

    for (std::size_t i = 0; i < 12; i++)
    {
        auto item       = minui::make_layout<minui::grid_item>(grid);
        item->rule.size = { "1" };
        item->rule.pos  = { i / 3, i % 3 };

        auto box         = minui::make_layout<minui::box_layout>(item);
        box->rule.shrink = { "20" };

        auto rr = minui::make_layout<minui_ms::rounded_rectangle>(box);
        rr->rectangleColor      = { 80, 80, 80, 255 };
        rr->shadowColor         = { 0, 0, 0, 0 };
        rr->config.cornerRadius = { 10.0f, 10.0f, 10.0f, 10.0f };
    }

    while (!WindowShouldClose())
    {
        rebound(root);
        review(root);
        update(root);

        BeginDrawing();
        ClearBackground(Color { 36, 36, 36, 255 });

        draw(root);

        if (minui::draw_debug) DrawFPS(0, 0);

        EndDrawing();
    }

    minui_ms::unload_minui();

    CloseWindow();
}
