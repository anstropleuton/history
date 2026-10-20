/* Anstro's Game of Life remake - Raylib edition
raylib-edition implementations - drawing and user interface

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

#ifndef ANSTRO_GAME_OF_LIFE_REMAKE_RAYLIB_EDITION_H
#define ANSTRO_GAME_OF_LIFE_REMAKE_RAYLIB_EDITION_H

#include "main.h"

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#ifndef AGOLR_RE_API
#define AGOLR_RE_API inline
#endif

// The game state to determine how the game will run
typedef struct State {
    int stepsPerFrame;      // Steps per frame, how many generations to calculate per frame (when negative, how many frames to render per generations)
    unsigned char frameCounter; // Number of frames from the start of the game (overflow is not a concern)
    bool paused;            // Is the game paused
    bool singleStep;        // Force a generation calculation
    bool showHelp;          // DIsplay a help message
    bool mouseLock;         // Disable mouse controls of the grid, to prevent accidentally making a cell live or dead when the mouse is on top of a GUI
    bool keyboardLock;      // Disable keyboard controls of the state, to prevent accidental change in state when the keyboard is in focus of GUI
    bool isGuiPanelOpen;    // Determines if a GUI panel (main control window) is open
    bool fadeAnimation;     // Determines if the fading should be applied
} State;

// Game contains the life and state, some functions edit both at once
typedef struct Game {
    Life *life;
    State state;
} Game;

// Get raygui's guiAlpha global variable
AGOLR_RE_API float GuiGetAlpha();

// Update the state of game
AGOLR_RE_API void UpdateGame(Game *game);

// Update the keyboard based controls
AGOLR_RE_API void UpdateKeyboardControls(Game *game);

// Update the mouse based controls
AGOLR_RE_API void UpdateMouseControls(Game *game);

// Update keyboard based and mouse based controls
AGOLR_RE_API void UpdateKeyboardWithMouseControls(Game *game);

// Custom GUI Control for value
AGOLR_RE_API void GuiControlledValueBox(Rectangle bounds, Vector2Int range, int *value, bool *editMode);

// Update the GUI based controls
// TODO: Fix text appearance on some style (like sunny)
AGOLR_RE_API void DrawGuiControls(Game *game);

// Determine the color with mixture
// TODO: Add mixture feature
AGOLR_RE_API Color GetCellColor(Life *life, int cellIndex, Color liveColor, Color deadColor);

// Draw the cells of the game
AGOLR_RE_API void DrawGame(Game *game, Color liveColor, Color deadColor);

#ifdef ANSTRO_GAME_OF_LIFE_REMAKE_RAYLIB_EDITION_IMPLEMENTATION

// Get raygui's guiAlpha global variable
float GuiGetAlpha()
{
    return guiAlpha;
}

// Update the state of game
void UpdateGame(Game *game)
{
    if (!game) return;

    if (game->state.stepsPerFrame > 0)
    {
        // For positive steps, step the generation n times
        for (int i = 0; i < game->state.stepsPerFrame; i++) StepGeneration(game->life);
    }
    else if (game->state.stepsPerFrame < 0)
    {
        // For negative steps, render the game n times before a step in generation
        if ((game->state.frameCounter) % (-game->state.stepsPerFrame) == 0) StepGeneration(game->life);
    }
}

// Update the keyboard based controls
void UpdateKeyboardControls(Game *game)
{
    if (!game) return;

    if (IsKeyPressed(KEY_DELETE)) ClearLife(game->life);

    // Button keys to change steps per frame
    if (IsKeyPressed(KEY_UP)) game->state.stepsPerFrame++;
    if (IsKeyPressed(KEY_DOWN)) game->state.stepsPerFrame--;

    if (IsKeyPressed(KEY_SPACE)) game->state.paused = !game->state.paused;
    if (IsKeyPressed(KEY_F1)) game->state.showHelp = !game->state.showHelp;
    game->state.singleStep = IsKeyPressed(KEY_ENTER);
}

// Update the mouse based controls
void UpdateMouseControls(Game *game)
{
    if (!game) return;

    Vector2Int cellPosition = { GetMousePosition().x * game->life->gridSize.x / GetScreenWidth(), GetMousePosition().y * game->life->gridSize.y / GetScreenHeight() };
    CELL_TYPE *cell = &game->life->cells[GetCellIndex(game->life, cellPosition)];

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) *cell = live;
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) *cell = fade;
}

// Update keyboard based and mouse based controls
void UpdateKeyboardWithMouseControls(Game *game)
{
    if (!game) return;

    // Control scroll to change steps per frame
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) game->state.stepsPerFrame += GetMouseWheelMove();
}

// Custom GUI Control for value
void GuiControlledValueBox(Rectangle bounds, Vector2Int range, int *value, bool *editMode)
{
    if (!value || !editMode) return;

    Vector2 mousePosition = GetMousePosition();

    // Left and right buttons for value adjustment
    if (GuiButton((Rectangle){ bounds.x                       , bounds.y, 20.0f, bounds.height }, "#116#")) *value--;
    if (GuiButton((Rectangle){ bounds.x + bounds.width - 20.0f, bounds.y, 20.0f, bounds.height }, "#117#")) *value++;

    // Main value box
    Rectangle valueBounds = { bounds.x + 25.0f, bounds.y, bounds.x + bounds.width - 75.0f, bounds.height };
    if (GuiValueBox(valueBounds, "", value, range.x, range.y, *editMode)) *editMode = !*editMode;

    // Speed up scrolling values
    float scrollSpeed = 1.0f;
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) scrollSpeed *= 5.0f;
    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) scrollSpeed *= 2.0f;

    // Adjusting value box with scrolling
    if (CheckCollisionPointRec(mousePosition, bounds) || *editMode) *value += GetMouseWheelMove() * scrollSpeed;

    // Keyboard adjustment
    if (*editMode)
    {
        if (IsKeyPressed(KEY_UP)) *value++;
        if (IsKeyPressed(KEY_DOWN)) *value--;
    }
}

// Display help window and draw a help message about the usage
void DrawGuiHelpWindow(Game *game)
{
    if (!game) return;

    // Draw a small button to open the help window
    if (!game->state.showHelp)
    {
        // Button to open help window
        Rectangle showHelpButtonBounds = (Rectangle){ GetScreenWidth() - 40, 20, 20, 20 };
        if (GuiButton(showHelpButtonBounds, "#191#")) game->state.showHelp = true;

        // NOTE: We set mouse lock to make sure that no cell interaction is made when clicking on this button
        if (CheckCollisionPointRec(GetMousePosition(), showHelpButtonBounds)) game->state.mouseLock = true;
        return;
    }

    const char *text = ""
            "Usage:\n"
            "  Mouse controls:\n"
            "    Hold shift to get more Information on the open panel button\n"
            "    Use middle mouse button on it to pause the game\n"
            "    Scroll while hovering on it to change steps per frame\n"
            "  keyboard controls:\n"
            "    Use Space bar to pause the game\n"
            "    Hold control while scrolling to change steps per frame\n"
            "    Press delete button to clear the grid";

    // Get text lines ('\n' delimiter) to process lines individually
    int lineCount = 0;
    const char **lines = GetTextLines(text, &lineCount);
    float totalHeight = (float)(lineCount*GuiGetStyle(DEFAULT, TEXT_SIZE) + (lineCount - 1)*GuiGetStyle(DEFAULT, TEXT_SIZE)/2);

    Rectangle helpWindowBounds = { GetScreenWidth() - 620, 20, 600, totalHeight + 40 };
    if (GuiWindowBox(helpWindowBounds, "Help")) game->state.showHelp = false;
    GuiDrawText(text, (Rectangle){ helpWindowBounds.x + 25, helpWindowBounds.y + 20, helpWindowBounds.width - 50, helpWindowBounds.height - 20 }, TEXT_ALIGN_LEFT, Fade(GetColor(GuiGetStyle(BUTTON, TEXT)), GuiGetAlpha()));
    if (CheckCollisionPointRec(GetMousePosition(), helpWindowBounds)) game->state.mouseLock = true;
}

// Display settings window with buttons
void DrawGuiSettingsPanel(Game *game)
{
    if (!game) return;

    // If the GUI settings panel is not open, use the versatile button with multiple function
    if (!game->state.isGuiPanelOpen)
    {
        // Account for tool tip
        GuiEnableTooltip();
        GuiSetTooltip(TextFormat("Open settings window%s", ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? TextFormat(", state: %s, SPF: %i", game->state.paused ? "Paused" : "Not paused", game->state.stepsPerFrame) : "")));

        Rectangle windowOpenButtonBounds = (Rectangle){ 20, 20, 20, 20 };
        if (GuiButton(windowOpenButtonBounds, "#80#")) game->state.isGuiPanelOpen = true;

        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (game->state.showHelp) game->state.showHelp = false;
            else game->state.isGuiPanelOpen = true;
        }

        // Additional functionality on the button
        // Middle click to pause
        // Scroll on it to change the steps per frame
        if (CheckCollisionPointRec(GetMousePosition(), windowOpenButtonBounds))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) game->state.paused = !game->state.paused;
            game->state.stepsPerFrame += GetMouseWheelMove();

            // NOTE: We set mouse lock to make sure that no cell interaction is made when clicking on this button
            game->state.mouseLock = true;
        }

        // Disable tooltip
        GuiDisableTooltip();
        return;
    }

    // GUI Settings Panel
    Rectangle windowBoxBounds = { 20, 20, 140, 275 };
    if (GuiWindowBox(windowBoxBounds, "Settings")) game->state.isGuiPanelOpen = false;

    // GUI Game state panel
    const char *pauseButtonText = (game->state.paused ? "Resume" : "Pause");
    if (GuiButton((Rectangle){ 25, 50, 130, 20 }, pauseButtonText)) game->state.paused = !game->state.paused;

    // Steps per frame control
    GuiDrawText("Steps per Frame", (Rectangle){ 25, 70, 130, 20 }, TEXT_ALIGN_CENTER, Fade(GetColor(GuiGetStyle(BUTTON, TEXT)), GuiGetAlpha()));
    static bool stepsPerFrameValueEditMode = false;
    GuiControlledValueBox((Rectangle){ 25, 90, 130, 20 }, (Vector2Int){ -1000, 1000 }, &game->state.stepsPerFrame, &stepsPerFrameValueEditMode);
    if (stepsPerFrameValueEditMode) game->state.keyboardLock = true;

    // Grid X and Y settings
    static Vector2Int gridSize = { 0 };
    static bool unsync = false;
    if ((gridSize.x != game->life->gridSize.x || gridSize.y != game->life->gridSize.y) && !unsync) gridSize = game->life->gridSize;
    GuiDrawText("Grid Size", (Rectangle){ 25, 110, 130, 20 }, TEXT_ALIGN_CENTER, Fade(GetColor(GuiGetStyle(BUTTON, TEXT)), GuiGetAlpha()));

    // Grid X control
    static bool gridSizeXValueEditMode = false;
    GuiControlledValueBox((Rectangle){ 25, 130, 130, 20 }, (Vector2Int){ 0, 1000000 }, &gridSize.x, &gridSizeXValueEditMode);
    if (gridSizeXValueEditMode) game->state.keyboardLock = true;

    // Grid Y control
    static bool gridSizeYValueEditMode = false;
    GuiControlledValueBox((Rectangle){ 25, 155, 130, 20 }, (Vector2Int){ 0, 1000000 }, &gridSize.y, &gridSizeYValueEditMode);
    if (gridSizeYValueEditMode) game->state.keyboardLock = true;

    // Post grid X and Y settings
    if (gridSize.x != game->life->gridSize.x || gridSize.y != game->life->gridSize.y) unsync = true;
    if (GuiButton((Rectangle){ 25, 180, 130, 20 }, "Apply Size"))
    {
        ResizeLife(game->life, gridSize);
    }

    // Draw FPS
    GuiDrawText(TextFormat("FPS: %i", GetFPS()), (Rectangle){ 25, 200, 130, 20 }, TEXT_ALIGN_CENTER, Fade(GetColor(GuiGetStyle(BUTTON, TEXT)), GuiGetAlpha()));

    // GUI Control to clear the grid
    if (GuiButton((Rectangle){ 25, 220, 130, 20 }, "Clear grid")) ClearLife(game->life);

    // GUI Control to show help
    if (GuiButton((Rectangle){ 25, 245, 130, 20 }, "Help")) game->state.showHelp = !game->state.showHelp;

    // GUI Control to toggle fading animations
    GuiToggle((Rectangle){ 25, 270, 130, 20 }, "Toggle Fade", &game->state.fadeAnimation);

    // Disallow mouse placement when the mouse is on top of the GUI settings panel
    if (CheckCollisionPointRec(GetMousePosition(), windowBoxBounds)) game->state.mouseLock = true;

    // When tab is pressed, tabulate between value box controls
    if (IsKeyPressed(KEY_TAB))
    {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
        {
        if (stepsPerFrameValueEditMode) { stepsPerFrameValueEditMode = false; gridSizeYValueEditMode = true; }
        else if (gridSizeXValueEditMode) { gridSizeXValueEditMode = false; stepsPerFrameValueEditMode = true; }
        else if (gridSizeYValueEditMode) { gridSizeYValueEditMode = false; gridSizeXValueEditMode = true; }
        else stepsPerFrameValueEditMode = true;
        }
        else
        {
        if (stepsPerFrameValueEditMode) { stepsPerFrameValueEditMode = false; gridSizeXValueEditMode = true; }
        else if (gridSizeXValueEditMode) { gridSizeXValueEditMode = false; gridSizeYValueEditMode = true; }
        else if (gridSizeYValueEditMode) { gridSizeYValueEditMode = false; stepsPerFrameValueEditMode = true; }
        else stepsPerFrameValueEditMode = true;
        }
    }

    // When escape is pressed, unfocus value box or close the GUI settings panel
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (stepsPerFrameValueEditMode) stepsPerFrameValueEditMode = false;
        else if (gridSizeXValueEditMode) gridSizeXValueEditMode = false;
        else if (gridSizeYValueEditMode) gridSizeYValueEditMode = false;
        else if (game->state.showHelp) game->state.showHelp = false;
        else game->state.isGuiPanelOpen = false;
    }
}

// Update the GUI based controls
// TODO: Fix text appearance on some style (like sunny)
void DrawGuiControls(Game *game)
{
    if (!game) return;

    game->state.mouseLock = false;
    game->state.keyboardLock = false;

    DrawGuiSettingsPanel(game);
    DrawGuiHelpWindow(game);

    if (!game->state.mouseLock && (!game->state.paused || IsMouseButtonDown(MOUSE_BUTTON_LEFT))) GuiFade(0.5f);
    else GuiFade(1.0f);
}

// Determine the color with mixture
// TODO: Add mixture feature
Color GetCellColor(Life *life, int cellIndex, Color liveColor, Color deadColor)
{
    if (!life) return BLANK;

    double initialRatio = 0.5;
    double finalRatio = 0.0;
    CELL_TYPE *cell = &life->cells[cellIndex];

    if (IsCellResolvedLive(life, cellIndex))
    {
        finalRatio = 0.0;
    }
    else
    {
        finalRatio = (2.0 * atan((double)(*cell - dead) / 10.0) / PI) * (1.0 - initialRatio) + initialRatio;
    }

    return (Color){
        liveColor.r * (1.0 - finalRatio) + deadColor.r * finalRatio,
        liveColor.g * (1.0 - finalRatio) + deadColor.g * finalRatio,
        liveColor.b * (1.0 - finalRatio) + deadColor.b * finalRatio,
        liveColor.a * (1.0 - finalRatio) + deadColor.a * finalRatio
    };
}

// Draw the cells of the game
void DrawGame(Game *game, Color liveColor, Color deadColor)
{
    if (!game) return;

    Vector2Int screenSize = { GetScreenWidth(), GetScreenHeight() };
    Vector2 cellSize = { (float)screenSize.x / game->life->gridSize.x, (float)screenSize.y / game->life->gridSize.y };

    // TODO: Another potential elemination of loop,
    // Combine this loop with the resolve generation
    for (int x = 0; x < game->life->gridSize.x; x++)
    {
        for (int y = 0; y < game->life->gridSize.y; y++)
        {
            Color cellColor;
            int cellIndex = GetCellIndex(game->life, (Vector2Int){ x, y });
            if (game->state.fadeAnimation) cellColor = GetCellColor(game->life, cellIndex, liveColor, deadColor);
            else if (IsCellResolvedLive(game->life, cellIndex)) cellColor = liveColor;
            else cellColor = deadColor;
            DrawRectangle(x * cellSize.x, y * cellSize.y, cellSize.x, cellSize.y, cellColor);
        }
    }

    // Tiny little rectangle showing which cell the mouse may interact with
    Vector2Int cellPosition = GetWrappedPosition((Vector2Int){ GetMousePosition().x * game->life->gridSize.x / GetScreenWidth(), GetMousePosition().y * game->life->gridSize.y / GetScreenHeight() }, game->life->gridSize);
    bool isCellResolvedLive = IsCellResolvedLive(game->life, GetCellIndex(game->life, cellPosition));
    if (isCellResolvedLive) { DrawRectangleLines(cellPosition.x * cellSize.x, cellPosition.y * cellSize.y, cellSize.x, cellSize.y, deadColor); DrawRectangleLines(cellPosition.x * cellSize.x - 1, cellPosition.y * cellSize.y - 1, cellSize.x + 2, cellSize.y + 2, liveColor); }
    else                    { DrawRectangleLines(cellPosition.x * cellSize.x, cellPosition.y * cellSize.y, cellSize.x, cellSize.y, liveColor); DrawRectangleLines(cellPosition.x * cellSize.x - 1, cellPosition.y * cellSize.y - 1, cellSize.x + 2, cellSize.y + 2, deadColor); }

    // Draw mouse cursor as a point to make things less annoying
    float cursorSize = fminf(cellSize.x, cellSize.y) / 10.0;
    if (isCellResolvedLive) { DrawCircleV(GetMousePosition(), cursorSize + 1, liveColor); DrawCircleV(GetMousePosition(), cursorSize, deadColor); }
    else                    { DrawCircleV(GetMousePosition(), cursorSize + 1, deadColor); DrawCircleV(GetMousePosition(), cursorSize, liveColor); }
}

#endif // ANSTRO_GAME_OF_LIFE_REMAKE_RAYLIB_EDITION_IMPLEMENTATION

#endif // ANSTRO_GAME_OF_LIFE_REMAKE_RAYLIB_EDITION_H