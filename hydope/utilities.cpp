/// @file
///
/// @author    Anstro Pleuton <https://github.com/anstropleuton>
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Hydope - Hypnotizing Double Pendulum.
///
/// This source file contains implementation for utilities.
///
/// This project is licensed under the terms of MIT license.

#include "utilities.hpp"
//
#include "rlgl.h"

bool IsHotkeyPressed(int hotkey)
{
    bool result = true;
    result &= ((hotkey & HOTKEY_SHIFT) != 0) == (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
    result &= ((hotkey & HOTKEY_CONTROL) != 0) == (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL));
    result &= ((hotkey & HOTKEY_ALT) != 0) == (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT));
    result &= ((hotkey & HOTKEY_SUPER) != 0) == (IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER));
    return result;
}

Vector2 GetPreviousMousePosition()
{
    Vector2 position = GetMousePosition();
    Vector2 delta    = GetMouseDelta();
    return position - delta;
}

int WrappingMod(int a, int b)
{
    return (a % b + b) % b;
}

void DrawTextureDest(Texture2D texture, Rectangle dest, Color tint)
{
    DrawTexturePro(texture, { 0.0f, 0.0f, (float) texture.width, (float) texture.height }, dest, { 0.0f, 0.0f }, 0.0f, tint);
}

void DrawTextureDestEx(Texture2D texture, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
    DrawTexturePro(texture, { 0.0f, 0.0f, (float) texture.width, (float) texture.height }, dest, origin, rotation, tint);
}

void DrawTriangleGradient(Vector2 p1, Vector2 p2, Vector2 p3, Color c1, Color c2, Color c3)
{
    rlBegin(RL_TRIANGLES);

    rlColor4ub(c1.r, c1.g, c1.b, c1.a);
    rlVertex2f(p1.x, p1.y);

    rlColor4ub(c2.r, c2.g, c2.b, c2.a);
    rlVertex2f(p2.x, p2.y);

    rlColor4ub(c3.r, c3.g, c3.b, c3.a);
    rlVertex2f(p3.x, p3.y);

    rlEnd();
}

float GetRoundnessFromRadius(float radius, Rectangle rec)
{
    float minDim = (rec.width > rec.height) ? rec.height : rec.width;
    if (minDim <= 0.0f) return 0.0f; // Avoid division by zero
    float roundness = (2.0f * radius) / minDim;
    if (roundness > 1.0f) roundness = 1.0f;
    if (roundness < 0.0f) roundness = 0.0f;
    return roundness;
}

void DrawRectangleRoundedRadius(Rectangle rec, float radius, int segments, Color color)
{
    DrawRectangleRounded(rec, GetRoundnessFromRadius(radius, rec), segments, color);
}

void DrawRectangleRoundedRadiusLines(Rectangle rec, float radius, int segments, Color color)
{
    DrawRectangleRoundedLines(rec, GetRoundnessFromRadius(radius, rec), segments, color);
}

void DrawRectangleRoundedRadiusLinesEx(Rectangle rec, float radius, int segments, float lineThick, Color color)
{
    DrawRectangleRoundedLinesEx(rec, GetRoundnessFromRadius(radius, rec), segments, lineThick, color);
}