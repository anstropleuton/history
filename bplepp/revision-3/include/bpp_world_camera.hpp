/**
 *  @file
 *  @brief      BPP World Camera.
 *  @author     Anstro Pleuton (github.com/anstropleuton)
 *  @copyright  Copyright (c) 2025 Anstro Pleuton
 *
 *  BPP, formally known as Bad Piggies mod Leading Edge Plus Plus (BPLE++,
 *  BPLEPP, BPP), or Bad Piggies Prototype, is a recreation of the mod
 *  Bad Piggies mod Leading Edge in C++.
 *
 *  Due to poor optimizations for the game Bad Piggies, BPLE suffers from
 *  extreme lag as the creation size grows larger. This C++ recreation aims to
 *  fix that issue by using the PhysX physics engine and raylib graphics
 *  library.
 *
 *  The choice for PhysX is simple: to perfectly recreate the physics of the
 *  original game, which is written in Unity.
 *  The choice for raylib is arbitrary, but it is a simple and easy-to-use
 *  library and supports various platforms.
 *
 *  ---
 *
 *  BPP World Camera defines a 2D camera using raylib that allows freely moving
 *  around in the grid.
 */

#pragma once

#include "bpp_raylib_ex.hpp"
#include "raylib.h"
#include "raymath.h"

namespace bpp {

    struct WorldCamera2D {
        Camera2D camera = { .zoom = 100.0f };

        float actualZoom = 6.64385619;
        float zoomAmount = 0.25f;

        void update()
        {
            camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

            float currentCameraZoom = camera.zoom;
            actualZoom += GetMouseWheelMove() * zoomAmount * IsHotkeyPressed(0);
            float nextZoom = std::pow(2.0f, actualZoom);

            Vector2 mouseOffset = GetMousePosition() - camera.offset;
            float changeWithZoom = 1.0f / (currentCameraZoom == 0.0f ? 1.0f : currentCameraZoom) - 1.0f / (nextZoom == 0.0f ? 1.0f : nextZoom);
            Vector2 target = camera.target + mouseOffset * changeWithZoom;

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
} // namespace bpp