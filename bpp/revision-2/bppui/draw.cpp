#include "draw.hpp"

#include "raymath.h"

auto bppui::draw_box::on_draw() const -> void {
    const Rectangle bounds{get_bounds()};
    if (bounds.width == 0.0F || bounds.height == 0.0F) {
        return;
    }

    draw_rec_shadow(bounds, border_radius, shadow_radius, shadow_shrink, shadow_offset, shadow_color);
    draw_rec_rounded(bounds, border_radius, color);
    draw_rec_rounded_lines(bounds, border_radius, border_thickness, border);
}

auto bppui::draw_text::on_draw() const -> void {
    const Rectangle bounds{get_bounds()};
    const Vector2   b_pos{rec_pos(bounds)};
    const Vector2   b_size{rec_size(bounds)};

    SetTextLineSpacing(line_spacing); // Cannot restore as raylib does not expose a getter

    const Vector2 text_size(MeasureTextEx(font, text.c_str(), font_size, spacing));
    const Vector2 text_final_pos{b_pos + (b_size - text_size) * align};
    DrawTextEx(font, text.c_str(), text_final_pos, font_size, spacing, color);
}

auto bppui::draw_sprite::on_draw() const -> void {
    const Rectangle bounds{get_bounds()};
    const Vector2   b_pos{rec_pos(bounds)};
    const Vector2   b_size{rec_size(bounds)};
    const Vector2   tex_size{static_cast<float>(texture.width), static_cast<float>(texture.height)};
    if (tex_size.x == 0.0F || tex_size.y == 0.0F) {
        return;
    }

    Vector2 final_size{tex_size};

    switch (fit) {
    case sprite_fit::unchanged: break;

    case sprite_fit::stretch: final_size = b_size; break;

    case sprite_fit::contain: {
        const float scale{std::min(b_size.x / tex_size.x, b_size.y / tex_size.y)};
        final_size = tex_size * scale;
        break;
    }

    case sprite_fit::cover: {
        const float scale{std::max(b_size.x / tex_size.x, b_size.y / tex_size.y)};
        final_size = tex_size * scale;
        break;
    }
    }

    const Rectangle tex_src{pos_size_rec({}, tex_size)};
    const Vector2   tex_final_pos{b_pos + (b_size - final_size) * align};
    const Rectangle tex_dst{pos_size_rec(tex_final_pos, final_size)};

    DrawTexturePro(texture, tex_src, tex_dst, {}, rotation, tint);
}
