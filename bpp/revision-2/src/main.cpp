#include "bppui.hpp"
#include "raylib.h"

auto main() -> int {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1000, 600, "BPP UI Test");
    SetTargetFPS(60);
    SetExitKey(0);

    bool         light_theme{};

    bppui::style style{bppui::load_default_style({.light = light_theme})};
    bppui::mgr.set_style(style);

    // bppui::mgr.get_root()->append(bppui::main_menu());

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_KP_ENTER)) {
            bppui::mgr.set_debug_draw(!bppui::mgr.get_debug_draw());
        }

        if (IsKeyPressed(KEY_KP_0)) {
            unload_style(style);
            light_theme = !light_theme;
            style       = bppui::load_default_style({.light = light_theme});
            bppui::mgr.set_style(style);
        }

        bppui::mgr.update();

        BeginDrawing();
        ClearBackground(bppui::mgr.get_style().background_color);

        bppui::mgr.draw();

        EndDrawing();
    }

    unload_style(style);

    CloseWindow();
}
