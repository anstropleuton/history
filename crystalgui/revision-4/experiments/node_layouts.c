// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Experiment to test node layouts
// Warning: May not compile
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "crystalgui.h"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 768, "Experiment - Node Layouts");
    SetExitKey(0);
    SetTargetFPS(60);

    CguiNode *root = CG_FLEX_R(
        CGUI_ALIGN_BEGIN,
        CG_BASIC_EX(CguiCentered((Vector2){200.0f, 200.0f}), NULL),
        CG_BASIC_EX(CguiCentered((Vector2){200.0f, 200.0f}), NULL),
        CG_BASIC_EX(CguiCentered((Vector2){200.0f, 200.0f}), NULL),
        CG_BASIC_EX(CguiCentered((Vector2){200.0f, 200.0f}), NULL)
    );

    while (!WindowShouldClose()) {
        root->modelDirty     = true;
        root->transformDirty = true;
        root->quadDirty      = true;
        CguiModelNode(root);
        CguiUpdateNode(root);

        BeginDrawing();
        ClearBackground(BLACK);

        CguiDrawNode(root);
        CguiDebugDrawNode(root, 255);

        EndDrawing();
    }

    CguiDeleteNode(root);

    CloseWindow();
}
