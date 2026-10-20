#include <cmath>

#include "raylib.h"

// Has to be after raylib.h
#include "raymath.h"

struct WorldCamera2D {
    Camera2D camera { .zoom = 128.0f };

    float actual_zoom { 7.0f };
    float zoom_amount { 0.25f };

    void update();

    operator Camera2D();
};

WorldCamera2D::operator Camera2D()
{
    return camera;
}

void WorldCamera2D::update()
{
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    float current_camera_zoom = camera.zoom;
    actual_zoom += GetMouseWheelMove() * zoom_amount;
    float next_zoom = std::pow(2.0f, actual_zoom);

    Vector2 mouse_offset = Vector2Subtract(GetMousePosition(), camera.offset);
    float change_with_zoom = 1.0f / (current_camera_zoom == 0.0f ? 1.0f : current_camera_zoom) - 1.0f / (next_zoom == 0.0f ? 1.0f : next_zoom);
    Vector2 target = Vector2Add(camera.target, Vector2Scale(mouse_offset, change_with_zoom));

    target = Vector2Subtract(target, Vector2Scale(GetMouseDelta(), IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) / (next_zoom == 0.0f ? 1.0f : next_zoom)));
    camera.target = target;
    camera.zoom = next_zoom;
}

struct Sprite {
    Rectangle selection;
    Vector2 pivot;
    Vector2 uvPosition;
    Vector2 size;
    Rectangle uv;
};

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "const char *title");
    SetTargetFPS(60);
    ToggleFullscreen();

    WorldCamera2D camera;

    Texture atlas = LoadTexture("res/IngameAtlas.png");

    Sprite sprite = {
        .selection = { 256, 768, 128, 128 },
        .pivot = { 0, 0 },
        .uvPosition = { 287, 802 },
        .size = { 66, 78 },
        .uv = { 0.550293f, 0.5498047f, 0.03222656f, 0.03808594f },
    };

    // Correction for raylib
    sprite.uv.x *= atlas.width;
    sprite.uv.y *= atlas.height;
    sprite.uv.width *= atlas.width;
    sprite.uv.height *= atlas.height;
    sprite.uv.y = atlas.height - sprite.uv.y - sprite.uv.height;

    Vector2 scaleParam = { 0.4f, 0.4f };
    Vector2 pivotParam = { 0.0f, 0.0f };
    float rotation;

    while (!WindowShouldClose())
    {
        float multiplier = 0.01f;
        if (IsKeyDown(KEY_LEFT_CONTROL)) multiplier *= 10.0f;
        if (IsKeyDown(KEY_LEFT_SHIFT)) multiplier *= 10.0f;
        if (IsKeyDown(KEY_S))
        {
            if (IsKeyDown(KEY_X)) scaleParam.x += (IsKeyDown(KEY_EQUAL) - IsKeyDown(KEY_MINUS)) * multiplier;
            if (IsKeyDown(KEY_Y)) scaleParam.y += (IsKeyDown(KEY_EQUAL) - IsKeyDown(KEY_MINUS)) * multiplier;
        }
        if (IsKeyDown(KEY_P))
        {
            if (IsKeyDown(KEY_X)) pivotParam.x += (IsKeyDown(KEY_EQUAL) - IsKeyDown(KEY_MINUS)) * multiplier * 10.0f;
            if (IsKeyDown(KEY_Y)) pivotParam.y += (IsKeyDown(KEY_EQUAL) - IsKeyDown(KEY_MINUS)) * multiplier * 10.0f;
        }
        if (IsKeyDown(KEY_R))
        {
            rotation += (IsKeyDown(KEY_EQUAL) - IsKeyDown(KEY_MINUS)) * multiplier * 10.0f;
        }

        int num = (int)(scaleParam.x * (float)sprite.size.x);
        int num2 = (int)(scaleParam.y * (float)sprite.size.y);
        int num3 = sprite.selection.x + sprite.selection.width / 2;
        int num4 = sprite.selection.y + sprite.selection.height / 2;
        int num5 = sprite.uvPosition.x + sprite.size.x / 2;
        int num6 = sprite.uvPosition.y + sprite.size.y / 2;
        int num7 = num3 - num5;
        int num8 = num4 - num6;
        int num9 = (int)(scaleParam.x * (float)(num7 + sprite.pivot.x + pivotParam.x));
        int num10 = (int)(scaleParam.y * (float)(num8 + sprite.pivot.y + pivotParam.y));
        float num11 = (float)num * 10.0f / 768.0f;
        float num12 = (float)num2 * 10.0f / 768.0f;
        float num13 = -2.0f * (float)num9 * 10.0f / 768.0f;
        float num14 = -2.0f * (float)num10 * 10.0f / 768.0f;

        Vector3 vertices[] = {
            Vector3 { num13 - num11, num14 - num12, 0.0f },
            Vector3 { num13 - num11, num14 + num12, 0.0f },
            Vector3 { num13 + num11, num14 + num12, 0.0f },
            Vector3 { num13 + num11, num14 - num12, 0.0f }
        };

        Rectangle dest = {
            vertices[0].x, -vertices[2].y,
            vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y
        };

        camera.update();
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
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
        DrawLineV({ -0.5f, -0.5f }, { 0.5f, -0.5f }, BLUE);
        DrawLineV({ 0.5f, -0.5f }, { 0.5f, 0.5f }, BLUE);
        DrawLineV({ 0.5f, 0.5f }, { -0.5f, 0.5f }, BLUE);
        DrawLineV({ -0.5f, 0.5f }, { -0.5f, -0.5f }, BLUE);
        DrawTexturePro(atlas, sprite.uv, { 0.0f, 0.0f, dest.width, dest.height }, { -dest.x, -dest.y }, rotation, WHITE);
        DrawLineV({ dest.x, dest.y }, { dest.x + dest.width, dest.y }, PURPLE);
        DrawLineV({ dest.x, dest.y + dest.height }, { dest.x + dest.width, dest.y + dest.height }, PURPLE);
        DrawLineV({ dest.x, dest.y }, { dest.x, dest.y + dest.height }, PURPLE);
        DrawLineV({ dest.x + dest.width, dest.y }, { dest.x + dest.width, dest.y + dest.height }, PURPLE);
        // DrawTextPro(GetFontDefault(), TextFormat("Source: { %f, %f, %f, %f }, Destination: { %f, %f, %f, %f }, Scale: { %f, %f }, Pivot: { %f, %f }", sprite.uv.x, sprite.uv.y, sprite.uv.width, sprite.uv.height, dest.x, dest.y, dest.width, dest.height, scaleParam.x, scaleParam.y, pivotParam.x, pivotParam.y), { 0.0f, 0.0f }, {}, 0.0f, 0.01f, 0.001f, YELLOW);
        DrawCircleV({ -pivotParam.x / sprite.selection.width, -pivotParam.y / sprite.selection.height }, 0.01f, { 0, 200, 200, 255 });
        EndMode2D();
        EndDrawing();
    }

    UnloadTexture(atlas);
    CloseWindow();
}