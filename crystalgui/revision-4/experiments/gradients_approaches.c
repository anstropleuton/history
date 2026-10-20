// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Experiment to test different gradient approaches
// Warning: May not compile
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "crystalgui.h"
#include "raymath.h"

#include <float.h>
#include <math.h>

int main(void) {
    const int screenWidth  = 320;
    const int screenHeight = 192;

    InitWindow(screenWidth, screenHeight, "Experiment - Gradient Approaches");
    SetExitKey(0);
    SetTargetFPS(60);

    CguiGradientFill fill = {0};

    Image image = GenImageColor(screenWidth, screenHeight, BLANK);

    bool dirty    = true;
    int  dragging = -1;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (fill.stopsCount < CGUI_MAX_GRADIENT_STOPS) {
                Color color = (Color){255, 0, 0, 255};
                if (fill.stopsCount != 0) color = CguiColorFromHSLA(CguiColorToHSLA(CguiResolveGradientFill(&fill, GetMousePosition())).x, 1.0f, 0.5f, 1.0f);
                fill.stops[fill.stopsCount++] = (CguiGradientStop){
                    .color    = color,
                    .position = mouse,
                };
                dirty = true;
            }
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            if (fill.stopsCount > 0) {
                int   closest = 0;
                float best    = FLT_MAX;

                for (int i = 0; i < fill.stopsCount; i++) {
                    float dx = fill.stops[i].position.x - mouse.x;
                    float dy = fill.stops[i].position.y - mouse.y;
                    float d2 = dx * dx + dy * dy;

                    if (d2 < best) {
                        best    = d2;
                        closest = i;
                    }
                }

                for (int i = closest; i < fill.stopsCount - 1; i++) {
                    fill.stops[i] = fill.stops[i + 1];
                }

                fill.stopsCount--;
                dirty = true;
            }
        }

        if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
            if (fill.stopsCount > 0) {
                int   closest = 0;
                float best    = FLT_MAX;

                for (int i = 0; i < fill.stopsCount; i++) {
                    float dx = fill.stops[i].position.x - mouse.x;
                    float dy = fill.stops[i].position.y - mouse.y;
                    float d2 = dx * dx + dy * dy;

                    if (d2 < best) {
                        best    = d2;
                        closest = i;
                    }
                }

                dragging = closest;
            }
        }

        if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON) && dragging != -1) {
            fill.stops[dragging].position = mouse;
        }

        if (IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON) && dragging != -1) {
            dragging = -1;
            dirty    = true;
        }

        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f && fill.stopsCount > 0) {
            int   closest = 0;
            float best    = FLT_MAX;

            for (int i = 0; i < fill.stopsCount; i++) {
                float dx = fill.stops[i].position.x - mouse.x;
                float dy = fill.stops[i].position.y - mouse.y;
                float d2 = dx * dx + dy * dy;

                if (d2 < best) {
                    best    = d2;
                    closest = i;
                }
            }

            Vector4 hsla              = CguiColorToHSLA(fill.stops[closest].color);
            hsla.x                    = fmodf(hsla.x + wheel * 10.0f + 360.0f, 360.0f);
            fill.stops[closest].color = CguiColorFromHSLAV(hsla);
            dirty                     = true;
        }

        if (dirty) {
            for (int x = 0; x < screenWidth; x++) {
                for (int y = 0; y < screenHeight; y++) {
                    ImageDrawPixel(&image, x, y, CguiResolveGradientFill(&fill, (Vector2){(float)x, (float)y}));
                }
            }
            dirty = false;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        Color *pixels = (Color *)image.data;
        for (int y = 0; y < screenHeight; y++) {
            for (int x = 0; x < screenWidth; x++) {
                DrawPixel(x, y, pixels[y * screenWidth + x]);
            }
        }

        for (int i = 0; i < fill.stopsCount; i++) {
            DrawCircleV(fill.stops[i].position, 5.0f, WHITE);
            DrawCircleLinesV(
                fill.stops[i].position,
                7.0f,
                BLACK
            );
        }

        EndDrawing();
    }

    UnloadImage(image);
    CloseWindow();
    return 0;
}
