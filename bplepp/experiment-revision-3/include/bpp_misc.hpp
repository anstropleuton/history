/*
 * BPLE++ Miscellaneous.
 * Copyright (C) Anstro Pleuton 2025
 * Public Domain or MIT Licensed
 */

#pragma once

#include "bpp_math.hpp"

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
        bool result { true };
        result &= ((hotkey & HOTKEY_SHIFT) != 0) == (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
        result &= ((hotkey & HOTKEY_CONTROL) != 0) == (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL));
        result &= ((hotkey & HOTKEY_ALT) != 0) == (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT));
        result &= ((hotkey & HOTKEY_SUPER) != 0) == (IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER));
        return result;
    }

    inline Vec2 GetPreviousMousePosition()
    {
        Vec2 position { GetMousePosition() };
        Vec2 delta { GetMouseDelta() };
        return position - delta;
    }

    inline int WrappingMod(int a, int b)
    {
        return (a % b + b) % b;
    }

} // namespace bpp
