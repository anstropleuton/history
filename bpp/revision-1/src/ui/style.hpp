#pragma once

#include "raylib.h"
#include "utils.hpp"

/*
Icon ideas:
- Home
- Search
- Menu
- More
- Close
- Back
- Forward
- Up
- Down
- Add
- Remove
- Edit
- Delete
- Save
- Copy
- Paste
- Cut
- Undo
- Redo
- Refresh
- Download
- Upload
- Share
- Link
- Info
- Help
- Warning
- Error
- Success
- Lock
- Unlock
- User
- Users
- Settings
- Notification
- Mail
- Calendar
- Clock
- Favorite
- Bookmark
- Filter
- Sort
*/

namespace bppui {
    struct style_fonts final {
        Font regular{};
        Font bold{};
        Font italic{};
        Font bold_italic{};
    };

    struct style_glyphs final {
        Texture checkbox{};
    };

    struct style_text final {
        float font_size{};
        float spacing{};
        int   line_spacing{};

        static auto zeroes() -> style_text {
            return {
                .font_size    = 0.0F,
                .spacing      = 0.0F,
                .line_spacing = 0,
            };
        }

        static auto ones() -> style_text {
            return {
                .font_size    = 1.0F,
                .spacing      = 1.0F,
                .line_spacing = 1,
            };
        }
    };

    struct style_element final {
        hsl     background_color;
        hsl     border_color;
        float   border_thickness{};
        float   corner_radius{};
        hsl     shadow_color;
        float   shadow_radius{};
        float   shadow_shrink{};
        Vector2 shadow_offset{};

        static auto zeroes() -> style_element {
            return {
                .background_color = {0.0F, 0.0F, 0.0F, 0.0F},
                .border_color     = {0.0F, 0.0F, 0.0F, 0.0F},
                .border_thickness = 0.0F,
                .corner_radius    = 0.0F,
                .shadow_color     = {0.0F, 0.0F, 0.0F, 0.0F},
                .shadow_radius    = 0.0F,
                .shadow_shrink    = 0.0F,
                .shadow_offset    = {0.0F, 0.0F},
            };
        }

        static auto ones() -> style_element {
            return {
                .background_color = {1.0F, 1.0F, 1.0F, 1.0F},
                .border_color     = {1.0F, 1.0F, 1.0F, 1.0F},
                .border_thickness = 1.0F,
                .corner_radius    = 1.0F,
                .shadow_color     = {1.0F, 1.0F, 1.0F, 1.0F},
                .shadow_radius    = 1.0F,
                .shadow_shrink    = 1.0F,
                .shadow_offset    = {1.0F, 1.0F},
            };
        }
    };

    struct style final {
        style_fonts   fonts;
        style_glyphs  glyphs;

        style_text    large_text;
        style_text    medium_text;
        style_text    small_text;

        float         large_icon{};
        float         medium_icon{};
        float         small_icon{};

        hsl           foreground_color;
        hsl           background_color;
        hsl           active_color;
        hsl           accent_color;
        hsl           success_color;
        hsl           info_color;
        hsl           warning_color;
        hsl           error_color;

        style_element base;
        style_element hover_offset;
        style_element hover_scale;
        style_element held_offset;
        style_element held_scale;
        style_element focused_offset;
        style_element focused_scale;
        style_element disabled_offset;
        style_element disabled_scale;
        style_element bordered_offset;
        style_element bordered_scale;
        style_element shadowed_offset;
        style_element shadowed_scale;
        style_element layered_offset;
        style_element layered_scale;
    };

    struct default_style_custom final {
        bool  light{};
        float accent{270.0F};
        float radius{5.0F};
        float radius_diff{5.0F};
    };

    [[nodiscard]]
    auto load_default_style(default_style_custom custom = {}) -> style;

    auto unload_style(const style &style) -> void;

    [[nodiscard]]
    inline auto operator+(const style_text first, const style_text second) -> style_text {
        return {
            .font_size    = first.font_size + second.font_size,
            .spacing      = first.spacing + second.spacing,
            .line_spacing = first.line_spacing + second.line_spacing,
        };
    }

    [[nodiscard]]
    inline auto operator-(const style_text first, const style_text second) -> style_text {
        return {
            .font_size    = first.font_size - second.font_size,
            .spacing      = first.spacing - second.spacing,
            .line_spacing = first.line_spacing - second.line_spacing,
        };
    }

    [[nodiscard]]
    inline auto operator*(const style_text first, const style_text second) -> style_text {
        return {
            .font_size    = first.font_size * second.font_size,
            .spacing      = first.spacing * second.spacing,
            .line_spacing = first.line_spacing * second.line_spacing,
        };
    }

    [[nodiscard]]
    inline auto operator/(const style_text first, const style_text second) -> style_text {
        return {
            .font_size    = first.font_size / second.font_size,
            .spacing      = first.spacing / second.spacing,
            .line_spacing = first.line_spacing / second.line_spacing,
        };
    }

    [[nodiscard]]
    inline auto operator+(const style_element &first, const style_element &second) -> style_element {
        return {
            .background_color = first.background_color + second.background_color,
            .border_color     = first.border_color + second.border_color,
            .border_thickness = first.border_thickness + second.border_thickness,
            .corner_radius    = first.corner_radius + second.corner_radius,
            .shadow_color     = first.shadow_color + second.shadow_color,
            .shadow_radius    = first.shadow_radius + second.shadow_radius,
            .shadow_shrink    = first.shadow_shrink + second.shadow_shrink,
            .shadow_offset    = first.shadow_offset + second.shadow_offset,
        };
    }

    [[nodiscard]]
    inline auto operator-(const style_element &first, const style_element &second) -> style_element {
        return {
            .background_color = first.background_color - second.background_color,
            .border_color     = first.border_color - second.border_color,
            .border_thickness = first.border_thickness - second.border_thickness,
            .corner_radius    = first.corner_radius - second.corner_radius,
            .shadow_color     = first.shadow_color - second.shadow_color,
            .shadow_radius    = first.shadow_radius - second.shadow_radius,
            .shadow_shrink    = first.shadow_shrink - second.shadow_shrink,
            .shadow_offset    = first.shadow_offset - second.shadow_offset,
        };
    }

    [[nodiscard]]
    inline auto operator*(const style_element &first, const style_element &second) -> style_element {
        return {
            .background_color = first.background_color * second.background_color,
            .border_color     = first.border_color * second.border_color,
            .border_thickness = first.border_thickness * second.border_thickness,
            .corner_radius    = first.corner_radius * second.corner_radius,
            .shadow_color     = first.shadow_color * second.shadow_color,
            .shadow_radius    = first.shadow_radius * second.shadow_radius,
            .shadow_shrink    = first.shadow_shrink * second.shadow_shrink,
            .shadow_offset    = first.shadow_offset * second.shadow_offset,
        };
    }

    [[nodiscard]]
    inline auto operator/(const style_element &first, const style_element &second) -> style_element {
        return {
            .background_color = first.background_color / second.background_color,
            .border_color     = first.border_color / second.border_color,
            .border_thickness = first.border_thickness / second.border_thickness,
            .corner_radius    = first.corner_radius / second.corner_radius,
            .shadow_color     = first.shadow_color / second.shadow_color,
            .shadow_radius    = first.shadow_radius / second.shadow_radius,
            .shadow_shrink    = first.shadow_shrink / second.shadow_shrink,
            .shadow_offset    = first.shadow_offset / second.shadow_offset,
        };
    }
} // namespace ui
