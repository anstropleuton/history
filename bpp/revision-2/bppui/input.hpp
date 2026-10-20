#pragma once

#include "draw.hpp"
#include "node.hpp"

#include <optional>

/*
Plan:
- Button
- Toggle button
- Toggle switch
- Checkbox
- Radio
- Slider
- Number input
- Text input
- Multi-line text input
- Dropdown
*/

namespace bppui {
    class button final : public node {
    public:
        std::string                                     label;
        std::size_t                                     icon{};
        bool                                            is_toggle{};
        bool                                            is_secondary{};
        std::function<void(std::optional<bool> active)> callback;
        bool                                            is_disabled{};
        bool                                            toggled{};

        explicit button();

        [[nodiscard]]
        auto get_hoverable_type() const -> hoverable_type override;

        auto on_mouse_enter() -> void override;

        auto on_mouse_leave() -> void override;

        auto on_mouse_press(int button) -> void override;

        auto on_mouse_release(int button) -> void override;

        [[nodiscard]]
        auto get_focusable_type() const -> focusable_type override;

        auto on_focus_gained() -> void override;

        auto on_focus_lost() -> void override;

        auto on_key_pressed(int key) -> void override;

        auto on_key_released(int key) -> void override;

        auto on_update() -> void override;

    private:
        draw_box    *background_;
        draw_text   *label_;
        draw_sprite *icon_;
        bool         is_hovered_{};
        bool         is_mouse_held_{};
        bool         is_focused_{};
        bool         is_key_held_{};
        bool         activated_{};
    };

    class checkbox final : public node {
    public:
        std::string                                     label;
        std::size_t                                     icon{};
        bool                                            is_secondary{};
        std::function<void(std::optional<bool> active)> callback;
        bool                                            is_disabled{};
        bool                                            toggled{};

        explicit checkbox();

        [[nodiscard]]
        auto get_hoverable_type() const -> hoverable_type override;

        auto on_mouse_enter() -> void override;

        auto on_mouse_leave() -> void override;

        auto on_mouse_press(int button) -> void override;

        auto on_mouse_release(int button) -> void override;

        [[nodiscard]]
        auto get_focusable_type() const -> focusable_type override;

        auto on_focus_gained() -> void override;

        auto on_focus_lost() -> void override;

        auto on_key_pressed(int key) -> void override;

        auto on_key_released(int key) -> void override;

        auto on_update() -> void override;

    private:
        draw_box    *background_;
        draw_text   *label_;
        draw_sprite *icon_;
        bool         is_hovered_{};
        bool         is_mouse_held_{};
        bool         is_focused_{};
        bool         is_key_held_{};
        bool         activated_{};
    };
} // namespace ui
