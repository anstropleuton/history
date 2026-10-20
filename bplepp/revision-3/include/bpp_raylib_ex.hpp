/**
 *  @file
 *  @brief      BPP raylib Ex.
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
 *  BPP raylib Ex extends raylib functionality by introducing some utilities.
 */

#pragma once

#include "raylib.h"
#include "raymath.h"

namespace bpp {

    typedef enum {
        NOTKEY_NONE = 0x0,
        HOTKEY_SHIFT = 0x1,
        HOTKEY_CONTROL = 0x2,
        HOTKEY_ALT = 0x4,
        HOTKEY_SUPER = 0x8,
    } Hotkey;

    inline bool IsHotkeyPressed(int hotkey)
    {
        bool result = true;
        result &= ((hotkey & HOTKEY_SHIFT) != 0) == (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
        result &= ((hotkey & HOTKEY_CONTROL) != 0) == (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL));
        result &= ((hotkey & HOTKEY_ALT) != 0) == (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT));
        result &= ((hotkey & HOTKEY_SUPER) != 0) == (IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER));
        return result;
    }

    inline Vector2 GetPreviousMousePosition()
    {
        Vector2 position = GetMousePosition();
        Vector2 delta = GetMouseDelta();
        return position - delta;
    }

    inline int WrappingMod(int a, int b)
    {
        return (a % b + b) % b;
    }

    inline void DrawTextureDest(Texture2D texture, Rectangle dest, Color tint)
    {
        DrawTexturePro(texture, { 0.0f, 0.0f, (float)texture.width, (float)texture.height }, dest, { 0.0f, 0.0f }, 0.0f, tint);
    }

    inline void DrawTextureDestEx(Texture2D texture, Rectangle dest, Vector2 origin, float rotation, Color tint)
    {
        DrawTexturePro(texture, { 0.0f, 0.0f, (float)texture.width, (float)texture.height }, dest, origin, rotation, tint);
    }

    struct Sprite {
        Texture2D texture;
        Rectangle source;
        Vector2 origin;
        float rotation;
        Color tint;
    };

    inline void DrawSprite(Sprite sprite, Rectangle dest)
    {
        DrawTexturePro(sprite.texture, sprite.source, dest, sprite.origin, sprite.rotation, sprite.tint);
    }

    inline void DrawSpriteEx(Sprite sprite, Rectangle dest, Vector2 origin, float rotation, Color tint)
    {
        DrawTexturePro(sprite.texture, sprite.source, dest, sprite.origin + origin, sprite.rotation + rotation, ColorTint(sprite.tint, tint));
    }
} // namespace bpp
