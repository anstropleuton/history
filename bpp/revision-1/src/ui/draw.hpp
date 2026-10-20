#pragma once

#include "node.hpp"

namespace bppui {
    // Display nodes

    class draw_box final : public node {
    public:
        Color   color{};
        Color   border{};
        float   border_thickness{};
        float   border_radius{};
        Color   shadow_color{};
        float   shadow_radius{};
        float   shadow_shrink{};
        Vector2 shadow_offset{};

        explicit draw_box(const model &model = {});

        auto on_draw() const -> void override;
    };

    class draw_text final : public node {
    public:
        std::string text;
        Font        font{};
        float       font_size{};
        float       spacing{};
        int         line_spacing{};
        Color       color{};
        Vector2     align{};

        explicit draw_text(const model &model = {});

        auto on_draw() const -> void override;
    };

    enum class sprite_fit : std::uint8_t {
        unchanged,
        stretch,
        contain,
        cover
    };

    class draw_sprite final : public node {
    public:
        Texture    texture{};
        Vector2    align{};
        float      rotation{};
        sprite_fit fit{};
        Color      tint{WHITE};

        explicit draw_sprite(const model &model = {});

        auto on_draw() const -> void override;
    };
} // namespace ui
