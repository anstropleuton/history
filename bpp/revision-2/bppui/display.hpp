#pragma once

#include "draw.hpp"
#include "node.hpp"

/*
Plan:
- Box
- Label
- Sprite
- Progress bar
- Loading spinner
- Separator
*/

namespace bppui {
    class panel final : public node {
    public:
        bool is_focus_group{};

        explicit panel() = default;

        [[nodiscard]]
        auto get_focusable_type() const -> focusable_type override;

        auto on_update() -> void override;

    private:
        draw_box *box_{};
    };

    enum class font_variant : std::uint8_t {
        regular,
        bold,
        italic,
        bold_italic,
    };

    enum class text_size : std::uint8_t {
        small,
        medium,
        large,
    };

    class label final : public node {
    public:
        std::string  text;
        font_variant variant{};
        text_size    size{};
        Vector2      align{0.5F, 0.5F};

        explicit label() = default;

        auto on_update() -> void override;

    private:
        draw_text *text_{};
    };
} // namespace ui
