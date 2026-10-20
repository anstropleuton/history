// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Experiment to test and observe computed geometry fields expansion
// Warning: May not compile
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "crystalgui.h"
#include "raymath.h"

/*

<svg width="100" height="100" viewBox="0 0 100 100" fill="none" xmlns="http://www.w3.org/2000/svg">
<path d="M15 40.6777L32.6777 23L50.3553 40.6777L68.033 23L85.7107 40.6777L50.3553 76.033L15 40.6777Z" fill="#FD0000"/>
</svg>

*/

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(768, 768, "Experiment - Fields Expansion");
    SetExitKey(0);
    SetTargetFPS(60);

    CguiGeom geom = {0};
    CGUI_ARRAY_RESERVE(Vector2, geom.vertices, 6);

    CGUI_ARRAY_INSERT_MANY_BACK(
        Vector2, geom.vertices,
        (Vector2){15.0f, 40.6777f},
        (Vector2){32.6777f, 23.0f},
        (Vector2){50.3553f, 40.6777f},
        (Vector2){68.033f, 23.0f},
        (Vector2){85.7107f, 40.6777f},
        (Vector2){50.3553f, 76.033f},
    );

    CguiComputeGeom cGeom = CguiComputeFields(geom);

    cGeom = CguiResizeFields(cGeom, (CguiIRectangle){0, 0, 100, 100});

    float min = INFINITY;
    float max = -INFINITY;

    CGUI_ARRAY_FOR_EACH(float, cGeom.fields, field) {
        min = fminf(min, *field);
        max = fmaxf(max, *field);
    }

    TraceLog(LOG_INFO, "min: %f, max: %f", min, max);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 screen = CguiGetScreenSizeV();

        float scaleX = screen.x / cGeom.rec.width;
        float scaleY = screen.y / cGeom.rec.height;
        float scale  = fminf(scaleX, scaleY);

        Vector2 tileSize = {scale, scale};
        Vector2 gridSize = {tileSize.x * cGeom.rec.width, tileSize.y * cGeom.rec.height};
        Vector2 offset   = {(screen.x - gridSize.x) * 0.5f, (screen.y - gridSize.y) * 0.5f};

        for (int x = 0; x < cGeom.rec.width; x++) {
            for (int y = 0; y < cGeom.rec.height; y++) {
                int   index = y * cGeom.rec.width + x;
                float field = cGeom.fields[index];

                Rectangle tile = {offset.x + x * tileSize.x, offset.y + y * tileSize.y, tileSize.x, tileSize.y};

                DrawRectangleRec(tile, CguiLerpColor(BLACK, WHITE, (field - min) / (max - min)));
                CguiDrawTextCentered(GetFontDefault(), TextFormat("%.0f", field), tile, 10, 1.0f, (Color){127, 127, 127, 255});
            }
        }

        EndDrawing();
    }

    CloseWindow();
}
