#include "PxPhysicsAPI.h" // IWYU pragma: export
#include "raylib.h"       // IWYU pragma: keep

using namespace physx;

template <typename Enum>
concept EnumWithMax = requires {
    {
        Enum::MAX
    };
};

template <typename T, EnumWithMax Enum> struct erray : std::array<T, (std::size_t)Enum::MAX> { // Yes. No, shut up.
    constexpr T &operator[](Enum n) { return std::array<T, (std::size_t)Enum::MAX>::operator[](static_cast<std::size_t>(n)); }
    constexpr const T &operator[](Enum n) const { return std::array<T, (std::size_t)Enum::MAX>::operator[](static_cast<std::size_t>(n)); }

    constexpr T &at(Enum n) { return std::array<T, (std::size_t)Enum::MAX>::at(static_cast<std::size_t>(n)); }
    constexpr const T &at(Enum n) const { return std::array<T, (std::size_t)Enum::MAX>::at(static_cast<std::size_t>(n)); }
};

struct Vec2 : PxVec2 {
    using PxVec2::PxVec2;
    Vec2() = default;

    Vec2(PxVec2 v) : PxVec2 { v.x, v.y } {}
    Vec2(Vector2 v) : PxVec2 { v.x, v.y } {}

    operator Vector2() { return { x, y }; }
};

struct Vec3 : PxVec3 {
    using PxVec3::PxVec3;
    Vec3() = default;

    Vec3(PxVec3 v) : PxVec3 { v.x, v.y, v.z } {}
    Vec3(Vector3 v) : PxVec3 { v.x, v.y, v.z } {}

    operator Vector3() { return { x, y, z }; }
};

struct Vec4 : PxVec4 {
    using PxVec4::PxVec4;
    Vec4() = default;

    Vec4(PxVec4 v) : PxVec4 { v.x, v.y, v.z, v.w } {}
    Vec4(Vector4 v) : PxVec4 { v.x, v.y, v.z, v.w } {}

    operator Vector4() { return { x, y, z, w }; }
};

struct Vec2Int : PxVec2T<int> {
    using PxVec2T<int>::PxVec2T;
    Vec2Int() = default;

    Vec2Int(PxVec2T<int> v) : PxVec2T<int> { v.x, v.y } {}
    Vec2Int(Vec2 v) : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}
    Vec2Int(PxVec2 v) : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}
    Vec2Int(Vector2 v) : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}

    operator Vector2() { return { (float)x, (float)y }; }
    operator Vec2() { return { (float)x, (float)y }; }
    operator PxVec2() { return { (float)x, (float)y }; }
};

struct Vec3Int : PxVec3T<int> {
    using PxVec3T<int>::PxVec3T;

    Vec3Int() = default;

    Vec3Int(PxVec3T<int> v) : PxVec3T<int> { v.x, v.y, v.z } {}
    Vec3Int(Vec3 v) : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}
    Vec3Int(PxVec3 v) : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}
    Vec3Int(Vector3 v) : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}

    operator Vector3() { return { (float)x, (float)y, (float)z }; }
    operator Vec3() { return { (float)x, (float)y, (float)z }; }
    operator PxVec3() { return { (float)x, (float)y, (float)z }; }
};

struct Vec4Int : PxVec4T<int> {
    using PxVec4T<int>::PxVec4T;

    Vec4Int() = default;

    Vec4Int(PxVec4T<int> v) : PxVec4T<int> { v.x, v.y, v.z, v.w } {}
    Vec4Int(Vec4 v) : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}
    Vec4Int(PxVec4 v) : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}
    Vec4Int(Vector4 v) : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}

    operator Vector4() { return { (float)x, (float)y, (float)z, (float)w }; }
    operator Vec4() { return { (float)x, (float)y, (float)z, (float)w }; }
    operator PxVec4() { return { (float)x, (float)y, (float)z, (float)w }; }
};

struct Quat : PxQuat {
    using PxQuat::PxQuat;
    Quat() = default;

    Quat(PxQuat q) : PxQuat { q.x, q.y, q.z, q.w } {}
    Quat(Quaternion q) : PxQuat { q.x, q.y, q.z, q.w } {}

    operator Quaternion() { return { x, y, z, w }; }
};

struct Rect : Rectangle {
    using Rectangle::Rectangle;
    Rect() = default;

    Rect(Rectangle r) : Rectangle { r.x, r.y, r.width, r.height } {}
    Rect(float _x, float _y, float _width, float _height) : Rectangle { _x, _y, _width, _height } {}
    Rect(Vec2 scale) : Rectangle { 0.0f, 0.0f, scale.x, scale.y } {}
    Rect(Vec2 position, Vec2 scale) : Rectangle { position.x, position.y, scale.x, scale.y } {}

    Vec2 p() { return { x, y }; }

    void p(Vec2 position)
    {
        x = position.x;
        y = position.y;
    }

    Vec2 s() { return { width, height }; }

    void s(Vec2 scale)
    {
        width = scale.x;
        height = scale.y;
    }

    Vec2 min() { return p(); }

    Vec2 max() { return p() + s(); }
};

struct CColor : Color {
    using Color::Color;
    CColor() = default;

    CColor(Color c) : Color { c.r, c.g, c.b, c.a } {}
    CColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : Color { _r, _g, _b, _a } {}
    CColor(int _r, int _g, int _b, int _a) : Color { (unsigned char)_r, (unsigned char)_g, (unsigned char)_b, (unsigned char)_a } {} // F you narrowing conversion error
    CColor(int hex) : Color { GetColor(hex) } {}
    CColor(Vec4 normalized) : Color { ColorFromNormalized(normalized) } {}
    CColor(Vec3 hsv) : Color { ColorFromHSV(hsv.x, hsv.y, hsv.z) } {}

    int toInt() { return ColorToInt(*this); }
    Vec4 normalize() { return ColorNormalize(*this); };
    Vec3 hsv() { return ColorToHSV(*this); }

    void tint(CColor tint) { *this = ColorTint(*this, tint); }
    void brighten(float factor) { *this = ColorBrightness(*this, factor); }
    void contrast(float contrast) { *this = ColorContrast(*this, contrast); }
};