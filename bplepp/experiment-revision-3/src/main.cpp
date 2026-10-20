#include <format>
#define global

#include "bpp_engine.hpp"
#include "raylib.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "BPLE++");

    bpp::InitEngine();

    while (!WindowShouldClose())
    {
        bpp::UpdateEngine();

        BeginDrawing();
        ClearBackground(BLACK);

        bpp::DrawEngine();

        EndDrawing();
    }

    bpp::CloseEngine();
}
