/*
 * BPLE++ Mathematics.
 * Copyright (C) Anstro Pleuton 2025
 * Public Domain or MIT Licensed
 */

#pragma once

#include "PxPhysicsAPI.h" // IWYU pragma: keep
#include "raylib.h"

using namespace physx;

namespace bpp {

    struct Vec2 : PxVec2 {
        using PxVec2::PxVec2;
        Vec2() = default;

        Vec2(PxVec2 v)
            : PxVec2 { v.x, v.y } {}
        Vec2(Vector2 v)
            : PxVec2 { v.x, v.y } {}

        operator Vector2() const
        {
            return { x, y };
        }
    };

    struct Vec3 : PxVec3 {
        using PxVec3::PxVec3;
        Vec3() = default;

        Vec3(PxVec3 v)
            : PxVec3 { v.x, v.y, v.z } {}
        Vec3(Vector3 v)
            : PxVec3 { v.x, v.y, v.z } {}

        operator Vector3() const
        {
            return { x, y, z };
        }
    };

    struct Vec4 : PxVec4 {
        using PxVec4::PxVec4;
        Vec4() = default;

        Vec4(PxVec4 v)
            : PxVec4 { v.x, v.y, v.z, v.w } {}
        Vec4(Vector4 v)
            : PxVec4 { v.x, v.y, v.z, v.w } {}

        operator Vector4() const
        {
            return { x, y, z, w };
        }
    };

    struct Vec2Int : PxVec2T<int> {
        using PxVec2T<int>::PxVec2T;
        Vec2Int() = default;

        Vec2Int(PxVec2T<int> v)
            : PxVec2T<int> { v.x, v.y } {}
        Vec2Int(Vec2 v)
            : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}
        Vec2Int(PxVec2 v)
            : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}
        Vec2Int(Vector2 v)
            : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}

        operator Vector2() const
        {
            return { (float)x, (float)y };
        }
        operator Vec2() const
        {
            return { (float)x, (float)y };
        }
        operator PxVec2() const
        {
            return { (float)x, (float)y };
        }
    };

    struct Vec3Int : PxVec3T<int> {
        using PxVec3T<int>::PxVec3T;

        Vec3Int() = default;

        Vec3Int(PxVec3T<int> v)
            : PxVec3T<int> { v.x, v.y, v.z } {}
        Vec3Int(Vec3 v)
            : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}
        Vec3Int(PxVec3 v)
            : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}
        Vec3Int(Vector3 v)
            : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}

        operator Vector3() const
        {
            return { (float)x, (float)y, (float)z };
        }
        operator Vec3() const
        {
            return { (float)x, (float)y, (float)z };
        }
        operator PxVec3() const
        {
            return { (float)x, (float)y, (float)z };
        }
    };

    struct Vec4Int : PxVec4T<int> {
        using PxVec4T<int>::PxVec4T;

        Vec4Int() = default;

        Vec4Int(PxVec4T<int> v)
            : PxVec4T<int> { v.x, v.y, v.z, v.w } {}
        Vec4Int(Vec4 v)
            : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}
        Vec4Int(PxVec4 v)
            : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}
        Vec4Int(Vector4 v)
            : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}

        operator Vector4() const
        {
            return { (float)x, (float)y, (float)z, (float)w };
        }
        operator Vec4() const
        {
            return { (float)x, (float)y, (float)z, (float)w };
        }
        operator PxVec4() const
        {
            return { (float)x, (float)y, (float)z, (float)w };
        }
    };

    inline Vec2Int Round(Vec2 v)
    {
        return { (int)std::round(v.x), (int)std::round(v.y) };
    }

    inline Vec3Int Round(Vec3 v)
    {
        return { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) };
    }

    inline Vec4Int Round(Vec4 v)
    {
        return { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) };
    }

    struct Quat : PxQuat {
        using PxQuat::PxQuat;
        Quat() = default;

        Quat(PxQuat q)
            : PxQuat { q.x, q.y, q.z, q.w } {}
        Quat(Quaternion q)
            : PxQuat { q.x, q.y, q.z, q.w } {}

        operator Quaternion()
        {
            return { x, y, z, w };
        }
    };

    struct Rect : Rectangle {
        using Rectangle::Rectangle;
        Rect() = default;

        Rect(Rectangle r)
            : Rectangle { r.x, r.y, r.width, r.height } {}
        Rect(float _x, float _y, float _width, float _height)
            : Rectangle { _x, _y, _width, _height } {}
        Rect(Vec2 scale)
            : Rectangle { 0.0f, 0.0f, scale.x, scale.y } {}
        Rect(Vec2 position, Vec2 scale)
            : Rectangle { position.x, position.y, scale.x, scale.y } {}

        Vec2 p() const
        {
            return { x, y };
        }

        void p(Vec2 position)
        {
            x = position.x;
            y = position.y;
        }

        Vec2 s() const
        {
            return { width, height };
        }

        void s(Vec2 scale)
        {
            width = scale.x;
            height = scale.y;
        }

        Vec2 min() const
        {
            return p();
        }

        Vec2 max() const
        {
            return p() + s();
        }
    };

    struct BppColor : Color {
        using Color::Color;
        BppColor() = default;

        BppColor(Color c)
            : Color { c.r, c.g, c.b, c.a } {}
        BppColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
            : Color { _r, _g, _b, _a } {}
        BppColor(int _r, int _g, int _b, int _a)
            : Color { (unsigned char)_r, (unsigned char)_g, (unsigned char)_b, (unsigned char)_a } {} // F you narrowing conversion error
        BppColor(int hex)
            : Color { GetColor(hex) } {}
        BppColor(Vec4 normalized)
            : Color { ColorFromNormalized(normalized) } {}
        BppColor(Vec3 hsv)
            : Color { ColorFromHSV(hsv.x, hsv.y, hsv.z) } {}

        int toInt() const
        {
            return ColorToInt(*this);
        }
        Vec4 normalize() const
        {
            return ColorNormalize(*this);
        };
        Vec3 hsv() const
        {
            return ColorToHSV(*this);
        }

        void tint(BppColor tint)
        {
            *this = ColorTint(*this, tint);
        }
        void brighten(float factor)
        {
            *this = ColorBrightness(*this, factor);
        }
        void contrast(float contrast)
        {
            *this = ColorContrast(*this, contrast);
        }
    };

} // namespace bpp
