#include <format>

#include "engine.hpp"

int main()
{
    InitEngine();

    while (!WindowShouldClose())
    {
        UpdateEngine();
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        int temp {};
        for (auto &prefab : partPrefabs)
        {
            prefab.draw({ 0.0f, temp * 1.0f });
            temp++;
        }
        for (float i { -10.0f }; i <= 10.0f; i++)
        {
            if (i == 0.0f)
            {
                DrawLineV({ -10.0f, i }, { 10.0f, i }, BLUE);
                DrawLineV({ i, -10.0f }, { i, 10.0f }, BLUE);
            }
            else
            {
                DrawLineV({ -10.0f, i }, { 10.0f, i }, RED);
                DrawLineV({ i, -10.0f }, { i, 10.0f }, GREEN);
            }
        }
        EndMode2D();
        DrawText(std::format("camera.camera.zoom = {};", camera.camera.zoom).c_str(), 0, 0, 40, WHITE);
        EndDrawing();
    }

    CloseEngine();
}
