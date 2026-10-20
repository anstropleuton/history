/// @file
///
/// @author    Anstro Pleuton <https://github.com/anstropleuton>
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Hydope - Hypnotizing Double Pendulum.
///
/// This header file contains utilities for Hydope.
///
/// This project is licensed under the terms of MIT license.

#pragma once

#include <cmath>

#include "raylib.h"
#include "raymath.h"

/// Vector2 but with double.
struct Vector2Double {
    double x = 0.0; ///< X component.
    double y = 0.0; ///< Y component.

    // Inter-converters

    Vector2Double() = default;

    Vector2Double(double x, double y) : x(x), y(y)
    {}

    Vector2Double(Vector2 v) : x(v.x), y(v.y)
    {}

    operator Vector2() const
    {
        return { (float) x, (float) y };
    }
};

typedef enum {
    NOTKEY_NONE    = 0x0,
    HOTKEY_SHIFT   = 0x1,
    HOTKEY_CONTROL = 0x2,
    HOTKEY_ALT     = 0x4,
    HOTKEY_SUPER   = 0x8,
} Hotkey;

bool    IsHotkeyPressed(int hotkey);
Vector2 GetPreviousMousePosition();
int     WrappingMod(int a, int b);

void DrawTextureDest(Texture2D texture, Rectangle dest, Color tint);
void DrawTextureDestEx(Texture2D texture, Rectangle dest, Vector2 origin, float rotation, Color tint);

void DrawTriangleGradient(Vector2 p1, Vector2 p2, Vector2 p3, Color c1, Color c2, Color c3);

void DrawRectangleRoundedRadius(Rectangle rec, float radius, int segments, Color color);
void DrawRectangleRoundedRadiusLines(Rectangle rec, float radius, int segments, Color color);
void DrawRectangleRoundedRadiusLinesEx(Rectangle rec, float radius, int segments, float lineThick, Color color);

struct WorldCamera2D {
    Camera2D camera = {};

    float actualZoom = -0.415037499f;
    float zoomAmount = 0.25f;

    void Update()
    {
        camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        float currentCameraZoom = std::pow(2.0f, actualZoom);
        actualZoom += GetMouseWheelMove() * zoomAmount * IsHotkeyPressed(0);
        float nextZoom = std::pow(2.0f, actualZoom);

        Vector2 mouseOffset    = GetMousePosition() - camera.offset;
        float   changeWithZoom = 1.0f / (currentCameraZoom == 0.0f ? 1.0f : currentCameraZoom) - 1.0f / (nextZoom == 0.0f ? 1.0f : nextZoom);
        Vector2 target         = camera.target + mouseOffset * changeWithZoom;

        float moveAmount = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) / (nextZoom == 0.0f ? 1.0f : nextZoom);
        target -= GetMouseDelta() * moveAmount;
        camera.target = target;

        camera.zoom = nextZoom;
    }

    Vector2 GetWorldPosition(Vector2 position)
    {
        return GetScreenToWorld2D(position, camera);
    }

    Vector2 GetScreenPosition(Vector2 position)
    {
        return GetWorldToScreen2D(position, camera);
    }

    Vector2 GetWorldMousePosition()
    {
        return GetScreenToWorld2D(GetMousePosition(), camera);
    }

    Vector2 GetWorldMouseDelta()
    {
        return GetScreenToWorld2D(GetMouseDelta(), camera);
    }

    Vector2 GetWorldPreviousMousePosition()
    {
        return GetScreenToWorld2D(GetPreviousMousePosition(), camera);
    }

    void BeginMode2D()
    {
        ::BeginMode2D(camera);
    }

    void EndMode2D()
    {
        ::EndMode2D();
    }

    operator Camera2D()
    {
        return camera;
    }
};
