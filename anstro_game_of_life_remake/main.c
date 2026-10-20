/* Anstro's Game of Life remake - Raylib edition
raylib-edition main file

Copyright (c) 2023 Anstro Pleuton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.  */

#define ANSTRO_GAME_OF_LIFE_REMAKE_IMPLEMENTATION
#include "main.h"

#define ANSTRO_GAME_OF_LIFE_REMAKE_RAYLIB_EDITION_IMPLEMENTATION
#include "raylib-edition.h"

// Init window and game
// Update game
// Draw grid
// Update and draw GUI
// Close window and game
int main(void)
{
    // Initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 1000, "Anstro's Game Of Life Remake");
    SetTargetFPS(60);
    SetExitKey(0);

    // Load a file style.rgs for GUI style
    GuiLoadStyle("style.rgs");

    Game game = { 0 };
    game.life = LoadLife((Vector2Int){ 100, 100 });

    game.state.stepsPerFrame = -10;
    game.state.frameCounter = 0;
    game.state.paused = true;
    game.state.showHelp = false;
    game.state.mouseLock = false;
    game.state.keyboardLock = false;
    game.state.isGuiPanelOpen = true;
    game.state.fadeAnimation = true;

    while (!WindowShouldClose())
    {
        // Control and update game
        if (!game.state.keyboardLock) UpdateKeyboardControls(&game);
        if (!game.state.mouseLock) UpdateMouseControls(&game);
        if (!game.state.keyboardLock && !game.state.mouseLock) UpdateKeyboardWithMouseControls(&game);
        if (!game.state.paused) UpdateGame(&game);
        if (game.state.singleStep) StepGeneration(game.life);

        if (game.state.mouseLock) ShowCursor();
        else                      HideCursor();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawGame(&game, GetColor(GuiGetStyle(BUTTON, BORDER)), GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            DrawGuiControls(&game);
        EndDrawing();

        game.state.frameCounter++;
    }

    // Deinitialization
    UnloadLife(game.life);
    CloseWindow();
}
