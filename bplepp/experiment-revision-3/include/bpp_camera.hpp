/*
 * BPLE++ Camera.
 * Copyright (C) Anstro Pleuton 2025
 * Public Domain or MIT Licensed
 */

#pragma once

#include "bpp_math.hpp"
#include "bpp_misc.hpp"
#include "raylib.h"

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

            Vec2 mouseOffset = { (Vec2)GetMousePosition() - (Vec2)camera.offset };
            float changeWithZoom = 1.0f / (currentCameraZoom == 0.0f ? 1.0f : currentCameraZoom) - 1.0f / (nextZoom == 0.0f ? 1.0f : nextZoom);
            Vec2 target = { (Vec2)camera.target + mouseOffset * changeWithZoom };

            float moveAmount = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) / (nextZoom == 0.0f ? 1.0f : nextZoom);
            target -= (Vec2)GetMouseDelta() * moveAmount;
            camera.target = target;

            camera.zoom = nextZoom;
        }

        Vec2 GetWorldPosition(Vec2 position)
        {
            return GetScreenToWorld2D(position, camera);
        }

        Vec2 GetScreenPosition(Vec2 position)
        {
            return GetWorldToScreen2D(position, camera);
        }

        Vec2 GetWorldMousePosition()
        {
            return GetScreenToWorld2D(GetMousePosition(), camera);
        }

        Vec2 GetWorldMouseDelta()
        {
            return GetScreenToWorld2D(GetMouseDelta(), camera);
        }

        Vec2 GetWorldPreviousMousePosition()
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
