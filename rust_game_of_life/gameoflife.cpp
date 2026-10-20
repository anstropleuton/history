/// @file
/// @copyright (c) 2025 Anstro Pleuton
/// Game of Life implementation in C++ using raylib.

#include <vector>

#include "raylib.h"
#include "raymath.h"

/// Individual cell in grid.
struct Cell {
    /// Cell state.
    enum State {
        Alive, ///< Cell is alive.
        Dead,  ///< Cell is dead.
        Born,  ///< Cell becomes alive in next frame.
        Killed ///< Cell becomes dead in next frame.
    };

    State state = Dead; ///< Cell state.
};

/// Grid of cells.
struct Grid {
    int               width  = 0; ///< Grid width.
    int               height = 0; ///< Grid height.
    std::vector<Cell> cells;      ///< Access pattern for cell in grid: cells[y * width + x]

    /// Create a new grid from size.
    static Grid New(int width, int height)
    {
        Grid grid = {
            .width  = width,
            .height = height,
            .cells  = std::vector<Cell>(width * height)
        };

        if (width >= 3 && height >= 3)
        {
            // Glider
            grid.GetCell(0, 0).state = Cell::Alive;
            grid.GetCell(1, 1).state = Cell::Alive;
            grid.GetCell(2, 1).state = Cell::Alive;
            grid.GetCell(0, 2).state = Cell::Alive;
            grid.GetCell(1, 2).state = Cell::Alive;
        }

        return grid;
    }

    /// Resize the grid by preserving as much data in order as possible.
    void Resize(int width, int height)
    {
        std::vector<Cell> newCells(width * height);

        int minWidth  = std::min(width, this->width);
        int minHeight = std::min(height, this->height);

        for (int x = 0; x < minWidth; x++)
        {
            for (int y = 0; y < minHeight; y++)
            {
                newCells[y * minWidth + x] = GetCell(x, y);
            }
        }

        this->width  = width;
        this->height = height;

        cells = newCells;
    }

    /// Get reference to cell at x, y.
    Cell &GetCell(int x, int y)
    {
        x = ((x % width) + width) % width;
        y = ((y % height) + height) % height;
        return cells[y * width + x];
    }

    /// Get number of alive neighbors.
    int Neighbors(int x, int y)
    {
        int neighbors = 0;

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0)
                {
                    continue;
                }

                int xx = x + dx;
                int yy = y + dy;

                if (GetCell(xx, yy).state == Cell::Alive || GetCell(xx, yy).state == Cell::Killed)
                {
                    neighbors++;
                }
            }
        }

        return neighbors;
    }

    /// Update all the cell states.
    void Update()
    {
        // Resolve next generation
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                int neighbors = Neighbors(x, y);
                if ((GetCell(x, y).state == Cell::Alive || GetCell(x, y).state == Cell::Killed) && (neighbors < 2 || neighbors > 3))
                {
                    GetCell(x, y).state = Cell::Killed;
                }
                if ((GetCell(x, y).state == Cell::Dead || GetCell(x, y).state == Cell::Born) && neighbors == 3)
                {
                    GetCell(x, y).state = Cell::Born;
                }
            }
        }

        // Apply next generation
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                if (GetCell(x, y).state == Cell::Born)
                {
                    GetCell(x, y).state = Cell::Alive;
                }
                if (GetCell(x, y).state == Cell::Killed)
                {
                    GetCell(x, y).state = Cell::Dead;
                }
            }
        }
    }
};

/// Main
int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1024, 768, "Game of Life");
    SetTargetFPS(60);

    Grid grid = Grid::New(10, 10);

    bool  running         = true;
    float intervalCounter = 0.0f;
    float updateInterval  = 0.1f;

    while (!WindowShouldClose())
    {
        // Keyboard controls

        if (IsKeyPressed(KEY_EQUAL))
        {
            updateInterval += 0.1f;
        }

        if (IsKeyPressed(KEY_MINUS))
        {
            updateInterval -= 0.1f;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            running = !running;
        }

        if (IsKeyPressed(KEY_UP))
        {
            grid.Resize(grid.width, grid.height - 1);
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            grid.Resize(grid.width, grid.height + 1);
        }

        if (IsKeyPressed(KEY_LEFT))
        {
            grid.Resize(grid.width - 1, grid.height);
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            grid.Resize(grid.width + 1, grid.height);
        }

        // Mouse control

        Vector2 screen    = { (float) GetScreenWidth(), (float) GetScreenHeight() };
        Vector2 cellUnit  = { screen.x / grid.width, screen.y / grid.height };
        int     selectedX = GetMousePosition().x / cellUnit.x;
        int     selectedY = GetMousePosition().y / cellUnit.y;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            grid.GetCell(selectedX, selectedY).state = Cell::Alive;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            grid.GetCell(selectedX, selectedY).state = Cell::Dead;
        }

        // Update

        if (running)
        {
            intervalCounter += GetFrameTime();
            if (intervalCounter >= updateInterval)
            {
                intervalCounter = 0.0f;
                grid.Update();
            }
        }

        // Draw

        BeginDrawing();
        ClearBackground({ 32, 32, 32, 255 });

        for (int x = 0; x < grid.width; x++)
        {
            for (int y = 0; y < grid.height; y++)
            {
                if (grid.GetCell(x, y).state == Cell::Alive)
                {
                    DrawRectangleRec({ cellUnit.x * x, cellUnit.y * y, cellUnit.x, cellUnit.y }, { 64, 64, 64, 255 });
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
}
