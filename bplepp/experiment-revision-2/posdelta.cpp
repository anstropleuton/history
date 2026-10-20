#include "engine.hpp"
#include "raylib.h"
#include "raymath.h"

struct WorldCamera2D {
    Camera2D camera { .zoom = 100.0f };

    float actualZoom { 6.64385619 };
    float zoomAmount { 0.25f };

    void update()
    {
        camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        auto currentCameraZoom = camera.zoom;
        actualZoom += GetMouseWheelMove() * zoomAmount;
        auto nextZoom = std::pow(2.0f, actualZoom);

        Vec2 mouseOffset { (Vec2)GetMousePosition() - (Vec2)camera.offset };
        auto changeWithZoom = 1.0f / (currentCameraZoom == 0.0f ? 1.0f : currentCameraZoom) - 1.0f / (nextZoom == 0.0f ? 1.0f : nextZoom);
        Vec2 target { (Vec2)camera.target + mouseOffset * changeWithZoom };

        auto moveAmount { IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) / (nextZoom == 0.0f ? 1.0f : nextZoom) };
        target -= (Vec2)GetMouseDelta() * moveAmount;
        camera.target = target;

        camera.zoom = nextZoom;
    }

    Vec2 getCameraPosition(Vec2 position)
    {
        return GetScreenToWorld2D(position, camera);
    }

    Vec2 getCameraMousePosition()
    {
        auto mousePosition = GetMousePosition();
        return GetScreenToWorld2D(mousePosition, camera);
    }

    Vec2 getCameraMouseDelta()
    {
        auto mouseDelta = GetMouseDelta();
        return GetScreenToWorld2D(mouseDelta, camera);
    }

    operator Camera2D()
    {
        return camera;
    }
};

int main()
{
    InitWindow(800, 800, "posdelta");
    SetTargetFPS(60);

    WorldCamera2D camera;

    while (!WindowShouldClose())
    {
        camera.update();

        Vec2 pos = GetMousePosition();
        Vec2 delta = GetMouseDelta();
        Vec2 sPos = GetScreenToWorld2D(pos, camera);
        Vec2 sDelta = GetScreenToWorld2D(delta, camera);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        DrawLineV({}, {100}, RED);
        DrawLineV({}, {0, 100}, RED);
        DrawCircleV(sPos, 1.0f, RED);
        DrawCircleV(sDelta, 1.0f, GREEN);
        DrawCircleV(Vec2(sPos - sDelta), 1.0f, BLUE);
        DrawCircleV(Vec2(sPos - delta / camera.camera.zoom), 1.0f, YELLOW);

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
}