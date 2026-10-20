// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Experiment to create inverted text over grayscale background
// Warning: May not compile
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "crystalgui.h"
#include "raymath.h"

#define REAL_GRAY CLITERAL(Color){127, 127, 127, 255}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 768, "Experiment - Inverted Text");
    SetExitKey(0);
    SetTargetFPS(60);

    const int steps = 100;

    while (!WindowShouldClose()) {
        BeginDrawing();

        Rectangle screen    = CguiGetScreenSizeRec();
        float     stepWidth = screen.width / steps;

        for (int i = 0; i < steps; i++) {
            float t = (float)i / (float)(steps - 1);

            Color background = CguiLerpColor(BLACK, WHITE, t);
            Color textColor  = t <= 0.5f ? CguiLerpColor(REAL_GRAY, WHITE, t / 0.5f) : CguiLerpColor(BLACK, REAL_GRAY, (t - 0.5f) / 0.5f);

            Rectangle slice = {screen.x + i * stepWidth, screen.y, stepWidth + 1, screen.height};

            CguiBeginScissorModeRec(slice);

            DrawRectangleRec(screen, background);

            CguiDrawTextCentered(GetFontDefault(), "Hello, everyone!", screen, 180, 1.0f, textColor);

            EndScissorMode();
        }

        EndDrawing();
    }

    CloseWindow();
}
