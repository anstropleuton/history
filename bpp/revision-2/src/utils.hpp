#pragma once

#include "raylib.h"
#include "raymath.h"

#include <cstdint>
#include <string>
#include <vector>

#ifndef BPP_GLOBAL
    #define BPP_GLOBAL extern
#endif

// Raylib utilities
[[nodiscard]]
inline auto rec_pos(const Rectangle rec) -> Vector2 {
    return {rec.x, rec.y};
}

[[nodiscard]]
inline auto rec_size(const Rectangle rec) -> Vector2 {
    return {rec.width, rec.height};
}

[[nodiscard]]
inline auto pos_size_rec(const Vector2 pos, const Vector2 size) -> Rectangle {
    return {pos.x, pos.y, size.x, size.y};
}

[[nodiscard]]
inline auto rec_center(const Rectangle rec) -> Vector2 {
    return rec_pos(rec) + rec_size(rec) / 2.0F;
}

[[nodiscard]]
inline auto top(const Vector4 tblr) -> float {
    return tblr.x;
}

[[nodiscard]]
inline auto bottom(const Vector4 tblr) -> float {
    return tblr.y;
}

[[nodiscard]]
inline auto left(const Vector4 tblr) -> float {
    return tblr.z;
}

[[nodiscard]]
inline auto right(const Vector4 tblr) -> float {
    return tblr.w;
}

[[nodiscard]]
inline auto rec_resize(const Rectangle rec, const Vector4 tblr) -> Rectangle {
    return {rec.x - top(tblr), rec.y - left(tblr), rec.width + top(tblr) + bottom(tblr), rec.height + left(tblr) + right(tblr)};
}

[[nodiscard]]
inline auto screen_size() -> Vector2 {
    return {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
}

[[nodiscard]]
inline auto screen_rec() -> Rectangle {
    return pos_size_rec({}, screen_size());
}

[[nodiscard]]
auto color_from_hsl(float hue, float sat, float lit, float alpha = 1.0F) -> Color;

[[nodiscard]]
inline auto color_from_hsl(const Vector3 hsl, const float alpha = 1.0F) -> Color {
    return color_from_hsl(hsl.x, hsl.y, hsl.z, alpha);
}

[[nodiscard]]
inline auto color_from_hsl(const Vector4 hsl) -> Color {
    return color_from_hsl(hsl.x, hsl.y, hsl.z, hsl.w);
}

[[nodiscard]]
auto color_to_hsl(Color color) -> Vector4;

class hsl final {
public:
    hsl() = default;

    explicit hsl(const Vector4 value) : hsl_(value) {
        color_ = color_from_hsl(hsl_);
    }

    explicit hsl(const Color color) : hsl_(color_to_hsl(color)) {
        color_ = color_from_hsl(hsl_);
    }

    explicit hsl(const Vector3 value, const float alpha = 1.0F) : hsl_(value.x, value.y, value.z, alpha) {
        color_ = color_from_hsl(hsl_);
    }

    hsl(const float hue, const float saturation, const float lightness, const float alpha = 1.0F) : hsl_(hue, saturation, lightness, alpha) {
        color_ = color_from_hsl(hsl_);
    }

    [[nodiscard]]
    auto get_hue() const -> float {
        return hsl_.x;
    }

    auto set_hue(const float hue) -> void {
        hsl_.x = hue;
    }

    [[nodiscard]]
    auto get_saturation() const -> float {
        return hsl_.y;
    }

    auto set_saturation(const float saturation) -> void {
        hsl_.y = saturation;
    }

    [[nodiscard]]
    auto get_lightness() const -> float {
        return hsl_.z;
    }

    auto set_lightness(const float lightness) -> void {
        hsl_.z = lightness;
    }

    [[nodiscard]]
    auto get_alpha() const -> float {
        return hsl_.w;
    }

    auto set_alpha(const float alpha) -> void {
        hsl_.w = alpha;
    }

    auto operator=(const Vector4 value) -> hsl & {
        hsl_   = value;
        color_ = color_from_hsl(hsl_);
        return *this;
    }

    auto operator=(const Color color) -> hsl & {
        hsl_   = color_to_hsl(color);
        color_ = color_from_hsl(hsl_);
        return *this;
    }

    auto operator=(const Vector3 value) -> hsl & {
        hsl_   = {value.x, value.y, value.z, 1.0F};
        color_ = color_from_hsl(hsl_);
        return *this;
    }

    auto operator()(const Vector4 value) -> void {
        hsl_   = value;
        color_ = color_from_hsl(hsl_);
    }

    auto operator()(const Color color) -> void {
        hsl_   = color_to_hsl(color);
        color_ = color_from_hsl(hsl_);
    }

    auto operator()(const Vector3 value, const float alpha = 1.0F) -> void {
        hsl_   = {value.x, value.y, value.z, alpha};
        color_ = color_from_hsl(hsl_);
    }

    auto operator()(const float hue, const float sat, const float lit, const float alpha = 1.0F) -> void {
        hsl_   = {hue, sat, lit, alpha};
        color_ = color_from_hsl(hsl_);
    }

    // ReSharper disable once CppNonExplicitConversionOperator
    operator Color() const { return color_; }

    // ReSharper disable once CppNonExplicitConversionOperator
    operator Vector4() const { return hsl_; }

private:
    Vector4       hsl_{};
    mutable Color color_{};
};

[[nodiscard]]
inline auto operator+(const hsl &first, const hsl &second) -> hsl {
    const auto first_hsl{static_cast<Vector4>(first)};
    const auto second_hsl{static_cast<Vector4>(second)};
    return hsl{first_hsl + second_hsl};
}

[[nodiscard]]
inline auto operator-(const hsl &first, const hsl &second) -> hsl {
    const auto first_hsl{static_cast<Vector4>(first)};
    const auto second_hsl{static_cast<Vector4>(second)};
    return hsl{first_hsl - second_hsl};
}

[[nodiscard]]
inline auto operator*(const hsl &first, const hsl &second) -> hsl {
    const auto first_hsl{static_cast<Vector4>(first)};
    const auto second_hsl{static_cast<Vector4>(second)};
    return hsl{first_hsl * second_hsl};
}

[[nodiscard]]
inline auto operator/(const hsl &first, const hsl &second) -> hsl {
    const auto first_hsl{static_cast<Vector4>(first)};
    const auto second_hsl{static_cast<Vector4>(second)};
    return hsl{first_hsl / second_hsl};
}

enum class hotkey : std::uint8_t {
    none    = 0,
    shift   = 1,
    control = 2,
    alt     = 4,
    super   = 8
};

[[nodiscard]]
inline auto operator|(const hotkey first, const hotkey second) -> hotkey {
    return static_cast<hotkey>(static_cast<std::uint8_t>(first) | static_cast<std::uint8_t>(second));
}

[[nodiscard]]
inline auto operator&(const hotkey first, const hotkey second) -> hotkey {
    return static_cast<hotkey>(static_cast<std::uint8_t>(first) & static_cast<std::uint8_t>(second));
}

[[nodiscard]]
inline auto is_key_repeated(const int key) -> bool {
    return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

[[nodiscard]]
inline auto is_hotkey_down(const hotkey hotk) -> bool {
    bool result{true};
    result &= static_cast<bool>(hotk & hotkey::shift) ? IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) : IsKeyUp(KEY_LEFT_SHIFT) && IsKeyUp(KEY_RIGHT_SHIFT);
    result &= static_cast<bool>(hotk & hotkey::control) ? IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL) : IsKeyUp(KEY_LEFT_CONTROL) && IsKeyUp(KEY_RIGHT_CONTROL);
    result &= static_cast<bool>(hotk & hotkey::alt) ? IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT) : IsKeyUp(KEY_LEFT_ALT) && IsKeyUp(KEY_RIGHT_ALT);
    result &= static_cast<bool>(hotk & hotkey::super) ? IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER) : IsKeyUp(KEY_LEFT_SUPER) && IsKeyUp(KEY_RIGHT_SUPER);
    return result;
}

[[nodiscard]]
inline auto is_key_combo_up(const hotkey hotk, const int key) -> bool { return is_hotkey_down(hotk) && IsKeyUp(key); }

[[nodiscard]]
inline auto is_key_combo_down(const hotkey hotk, const int key) -> bool { return is_hotkey_down(hotk) && IsKeyDown(key); }

[[nodiscard]]
inline auto is_key_combo_pressed(const hotkey hotk, const int key) -> bool { return is_hotkey_down(hotk) && IsKeyPressed(key); }

[[nodiscard]]
inline auto is_key_combo_released(const hotkey hotk, const int key) -> bool { return is_hotkey_down(hotk) && IsKeyReleased(key); }

[[nodiscard]]
inline auto is_key_combo_pressed_repeat(const hotkey hotk, const int key) -> bool { return is_hotkey_down(hotk) && IsKeyPressedRepeat(key); }

[[nodiscard]]
inline auto is_key_combo_repeated(const hotkey hotk, const int key) -> bool { return is_hotkey_down(hotk) && is_key_repeated(key); }

[[nodiscard]]
auto intersect_rec(const Rectangle &first, const Rectangle &second) -> Rectangle;

[[nodiscard]]
auto union_rec(const Rectangle &first, const Rectangle &second) -> Rectangle;

BPP_GLOBAL std::vector<Rectangle> clips;

inline auto begin_scissor_mode_rec(const Rectangle rec) {
    BeginScissorMode(static_cast<int>(rec.x), static_cast<int>(rec.y), static_cast<int>(rec.width), static_cast<int>(rec.height));
}

inline auto begin_clip() -> void {
    clips.clear();
    begin_scissor_mode_rec(screen_rec());
}

inline auto push_clip(const Rectangle rec) -> void {
    const Rectangle last{clips.empty() ? screen_rec() : clips.back()};
    const Rectangle intersect{intersect_rec(last, rec)};
    clips.push_back(intersect);
    EndScissorMode();
    begin_scissor_mode_rec(intersect);
}

inline auto pop_clip() -> void {
    if (clips.empty()) {
        return;
    }

    clips.pop_back();
    const Rectangle last{clips.empty() ? screen_rec() : clips.back()};
    EndScissorMode();
    begin_scissor_mode_rec(last);
}

inline auto end_clip() -> void {
    EndScissorMode();
}

auto draw_rec_rounded(Rectangle rec, float radius, Color color) -> void;

auto draw_rec_rounded_lines(Rectangle rec, float radius, float thickness, Color color) -> void;

auto draw_rec_shadow(Rectangle rec, float radius, float blur, float shrink, Vector2 offset, Color color) -> void;

[[nodiscard]]
auto load_svg(const std::string &filename, int width, int height) -> Texture;
