#include <raylib.h>

#include <cmath>
#include <cstdlib>
#include <limits>
#define APLIB_GAME
#include "aplib.hpp"
using namespace aplib::ansi;
using namespace aplib::game;

std::vector<vec2> shapes;

bool has_shape_at(vec2 pos)
{
    for (vec2 shape : shapes)
    {
        if (shape == pos)
        {
            return true;
        }
    }
    return false;
}

int main(void)
{
    std::cout << "Enter shapes count: ";
    size_t shapes_count = 0;
    std::cin >> shapes_count;
    float sqrt_sc = sqrt(shapes_count + 1.0f);

    if (std::cin.peek() == '\n') std::cout << "Seed: ";
    size_t seed = 0;
    std::cin >> seed;
    srand(seed);

    vec2 current = { 0.0f, 0.0f };
    for (size_t i = 0; i < shapes_count; i++)
    {
        bool still_moved = false;
        for (size_t j = shapes.size(); j-- > 0;)
        {
            vec2 direction = { 0.0f, 0.0f };
            bool moved = false;
            for (size_t k = 0; k < 4; k++)
            {
                switch (rand() % 4)
                {
                    case 0: direction = { +0.0f, +1.0f }; break;
                    case 1: direction = { +0.0f, -1.0f }; break;
                    case 2: direction = { +1.0f, +0.0f }; break;
                    case 3: direction = { -1.0f, +0.0f }; break;
                }
                if (!has_shape_at(current + direction))
                {
                    current += direction;
                    moved = true;
                    still_moved = true;
                    break;
                }
            }
            if (moved)
            {
                break;
            }
            current = shapes[i];
        }
        if (!still_moved)
        {
            current = { (float)(rand() % (int)sqrt_sc), (float)(rand() % (int)sqrt_sc) };
        }
        shapes.push_back(current);
    }

    vec2 min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    vec2 max = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };

    for (vec2 shape : shapes)
    {
        if (min.x > shape.x)
        {
            min.x = shape.x;
        }
        if (min.y > shape.y)
        {
            min.y = shape.y;
        }
        if (max.x < shape.x)
        {
            max.x = shape.x;
        }
        if (max.y < shape.y)
        {
            max.y = shape.y;
        }
    }

    vec2 start = vec2{ 0.0f, 0.0f } - min;
    vec2 size = max - min + vec2{ 1.0f, 1.0f };

    raylib_init("Random Shape", size.x / sqrt_sc, size.y / sqrt_sc);

    auto fmt = [](vec2 vec, std::string name) -> std::string
    {
        return name + ": " + std::to_string(vec.x) + ", " + std::to_string(vec.y) + '\n';
    };

    std::cout << fmt(min, "Min") << fmt(max, "Max") << fmt(start, "Start") << fmt(size, "Size");
    for (vec2 shape : shapes)
    {
        std::cout << fmt(shape, "Shape");
    }

    world_camera_2D cam;

    RenderTexture rt = LoadRenderTexture(size.x, size.y);

    BeginTextureMode(rt);
    ClearBackground(BLACK);
    for (size_t i = 0; i < shapes.size(); i++)
    {
        vec2 shape = shapes[i] + start;
        DrawPixel(shape.x, shape.y, ColorAlpha(WHITE, i / (shapes.size() * 2.0f + 1.0f) + 0.5f));
    }
    EndTextureMode();

    while (!WindowShouldClose())
    {
        cam.update();
        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(cam);
        DrawTexturePro(rt.texture, { 0.0f, 0.0f, (float)rt.texture.width, (float)-rt.texture.height }, { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() }, { 0.0f, 0.0f }, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(rt);

    raylib_close();
}