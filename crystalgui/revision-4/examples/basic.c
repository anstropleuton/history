// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Basic Crystal GUI usage example
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "crystalgui.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 768, "Hello, Crystal GUI!");
    SetExitKey(0);
    SetTargetFPS(60);

    CguiNode *node = CG_BASIC_EX(
        ((CguiModel){
            .posRef   = 0.0f,
            .sizeRef  = 0.0f,
            .offset   = {0.0f, 0.0f},
            .anchor   = {0.5f, 0.0f},
            .floating = {-0.5f, 0.0f},
            .extent   = {400.0f, 0.0f},
            .scale    = {0.0f, 1.0f},
        }),
        CG_BASIC_EX(
            ((CguiModel){
                .posRef   = 0.0f,
                .sizeRef  = 0.0f,
                .offset   = {0.0f, 0.0f},
                .anchor   = {0.0f, 0.0f},
                .floating = {0.0f, 0.0f},
                .extent   = {-20.0f, -20.0f},
                .scale    = {1.0f, 1.0f},
            }),
            CG_BASIC_EX(
                ((CguiModel){
                    .posRef   = 0.0f,
                    .sizeRef  = 0.0f,
                    .offset   = {0.0f, 0.0f},
                    .anchor   = {0.0f, -0.5f},
                    .floating = {0.0f, 0.5f},
                    .extent   = {0.0f, 200.0f},
                    .scale    = {1.0f, 0.0f},
                }),
                NULL
            ),
            CG_BASIC_EX(
                ((CguiModel){
                    .posRef   = 0.0f,
                    .sizeRef  = 0.0f,
                    .offset   = {0.0f, 0.0f},
                    .anchor   = {0.0f, 0.5f},
                    .floating = {0.0f, -0.5f},
                    .extent   = {0.0f, -220.0f},
                    .scale    = {1.0f, 1.0f},
                }),
                CG_BASIC_EX(
                    ((CguiModel){
                        .posRef   = 1.0f,
                        .sizeRef  = 1.0f,
                        .offset   = {0.0f, 0.0f},
                        .anchor   = {-0.5f, 0.5f},
                        .floating = {0.5f, -0.5f},
                        .extent   = {200.0f, 200.0f},
                        .scale    = {0.0f, 0.0f},
                    }),
                    NULL
                )
            )
        )
    );

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsWindowResized()) {
            node->modelDirty = true;
        }

        CguiModelNode(node);
        CguiUpdateNode(node);
        CguiDrawNode(node);
        CguiDebugDrawNode(node, 100);

        EndDrawing();
    }

    CguiDeleteNode(node);

    CloseWindow();
}
