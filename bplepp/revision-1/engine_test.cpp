#include "engine.hpp"

#include <vector>

extern Camera2D gCamera;

int main()
{
    engine_init();

    std::vector<part_t *> parts;

    while (!WindowShouldClose())
    {
        PxVec2 pos = engine_get_mouse_pos();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            parts.push_back(new part_t { { pos.x, pos.y } });
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            for (std::size_t i = parts.size(); i-- > 0;)
            {
                if (parts[i]->check_collision(pos))
                {
                    parts.erase(parts.begin() + i);
                }
            }
        }

        engine_iterate();
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(gCamera);

        for (auto &part : parts)
        {
            part->draw();
        }

        EndMode2D();
        EndDrawing();
    }

    for (auto &part : parts)
    {
        delete part;
    }

    engine_close();
}