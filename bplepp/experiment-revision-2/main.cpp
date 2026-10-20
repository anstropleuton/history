#include <cmath>
#include <random>

#include "engine.hpp"

int main()
{
    EngineInit();
    WorldCamera2D camera;
    BuildModePart templatePart { PartPrefabID::EGG, {}, 0, false };

    while (!WindowShouldClose())
    {
        camera.update();

        if (IsKeyPressed(KEY_ENTER))
        {
            SwitchGameMode();
        }

        bool buildMode { currentGameMode == GameMode::BUILD_MODE };

        EngineUpdate();

        BeginDrawing();
        ClearBackground(buildMode ? CColor { 135, 206, 235, 255 } : CColor { 25, 25, 72, 255 });
        terrain.draw(camera, buildMode);
        BeginMode2D(camera);
        EngineDraw();

        if (buildMode)
        {
            templatePart.position = WorldToGrid(camera.getWorldMousePosition());
            templatePart.draw(ColorAlpha(WHITE, 0.5f));
        }
        if (buildMode)
        {
            if (IsHotkeyPressed(HOTKEY_ALT))
            {
                templatePart.prefabId = (PartPrefabID)AbsMod((int)templatePart.prefabId + GetMouseWheelMove(), (int)PartPrefabID::MAX);
            }

            Vec2 mousePosition { GetMousePosition() };
            Vec2 mouseDelta { GetMouseDelta() };
            float magnitude { mouseDelta.magnitude() / camera.camera.zoom };
            for (float f { 0.0f }; f <= magnitude; f++)
            {
                Vec2 position { mousePosition - mouseDelta * (f / (magnitude == 0.0f ? 1.0f : magnitude)) };
                Vec2 worldPosition { camera.getWorldPosition(position) };
                Vec2Int gridPosition { WorldToGrid(worldPosition) };
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                {
                    if (!RemovePartAtPos(gridPosition, false))
                    {
                        RemovePartAtPos(gridPosition, true);
                    }
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    AddPartAtPos(gridPosition, templatePart);
                }
            }
        }

        EndMode2D();
        EndDrawing();
    }

    EngineClose();
}
