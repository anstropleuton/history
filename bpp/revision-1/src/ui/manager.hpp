#pragma once
#include "node.hpp"
#include "raylib.h"
#include "style.hpp"
#include "utils.hpp"

#include <memory>

#ifndef BPP_GLOBAL
    #define BPP_GLOBAL extern
#endif

namespace bppui {
    // Node manager

    class ui_manager final {
    public:
        explicit ui_manager(std::unique_ptr<node> root = std::make_unique<node>()) {
            if (root == nullptr) {
                root = std::make_unique<node>();
            }

            root_ = std::move(root);
        }

        ui_manager(const ui_manager &)                     = delete;
        ui_manager(ui_manager &&)                          = delete;

        auto operator=(const ui_manager &) -> ui_manager & = delete;
        auto operator=(ui_manager &&) -> ui_manager &      = delete;

        // Properties
    private:
        std::unique_ptr<node> root_;
        bool                  debug_draw_{};
        style                 style_;
        bool                  style_loaded_{};

    public:
        [[nodiscard]]
        auto get_root() -> node * {
            return root_.get();
        }

        [[nodiscard]]
        auto get_root() const -> const node * {
            return root_.get();
        }

        auto set_root(std::unique_ptr<node> root) -> void {
            root_ = std::move(root);
            if (root_ == nullptr) {
                root_ = std::make_unique<node>();
            }

            hovered_       = nullptr;
            captured_      = nullptr;
            focused_       = nullptr;
            focused_group_ = nullptr;

            root_->dirty_  = true;
        }

        [[nodiscard]]
        auto get_debug_draw() const -> bool {
            return debug_draw_;
        }

        auto set_debug_draw(const bool debug_draw) -> void {
            debug_draw_ = debug_draw;
        }

        [[nodiscard]]
        auto get_style() const -> const style & {
            return style_;
        }

        auto set_style(const style &style) -> void {
            style_ = style;
        }

        // Event middleman
    private:
        std::function<void(int button)>                               mouse_button_press_;
        std::function<void(int button)>                               mouse_button_release_;
        std::function<void(Vector2 pos, Vector2 prev, Vector2 delta)> mouse_move_;
        std::function<void(Vector2 delta)>                            mouse_wheel_move_;
        std::function<void(int key)>                                  keyboard_key_press_;
        std::function<void(int key)>                                  keyboard_key_release_;
        std::function<void(std::span<const int> chars)>               keyboard_key_text_;

    public:
        auto set_mouse_button_press(const decltype(mouse_button_press_) &callback) -> void {
            mouse_button_press_ = callback;
        }

        auto set_mouse_button_release(const decltype(mouse_button_release_) &callback) -> void {
            mouse_button_release_ = callback;
        }

        auto set_mouse_move(const decltype(mouse_move_) &callback) -> void {
            mouse_move_ = callback;
        }

        auto set_mouse_wheel_move(const decltype(mouse_wheel_move_) &callback) -> void {
            mouse_wheel_move_ = callback;
        }

        // Events
    private:
        node *hovered_{};
        node *captured_{};
        int   captured_with_{mouse_button_none};
        node *focused_{};
        node *focused_group_{};
        node *recently_focused_{};
        node *recently_focused_group_{};

        auto set_hovered_(node *new_node) -> void;

        [[nodiscard]]
        auto associated_focus_group_() const -> node *;

        [[nodiscard]]
        auto initial_focusable_in_group_(bool prefer_last) const -> node *;

        auto restore_focus_state_() -> bool;

        auto set_focused_(node *new_focusable) -> void;

        auto set_focused_group_(node *new_focus_group) -> void;

        // Mouse

        [[nodiscard]]
        auto hit_test_() const -> node *;

        static constexpr int mouse_button_none{-1};
        static constexpr int mouse_buttons_count{7};

        auto dispatch_hover_() -> void;

        auto dispatch_capture_() -> void;

        auto dispatch_uncapture_() -> void;

        auto dispatch_mouse_move_() const -> void;

        auto dispatch_mouse_wheel_move_() const -> void;

        auto dispatch_mouse_cb_() const -> void;

        auto dispatch_mouse_() -> void;

        // Keyboard

        // From raylib (inclusive, exclusive)
        static constexpr std::array key_ranges_{
            std::pair{32,  33 },
            std::pair{39,  40 },
            std::pair{44,  58 },
            std::pair{59,  60 },
            std::pair{61,  62 },
            std::pair{65,  94 },
            std::pair{96,  97 },
            std::pair{257, 258},
            std::pair{259, 262},
            std::pair{266, 268},
            std::pair{280, 285},
            std::pair{290, 302},
            std::pair{340, 349},
            std::pair{320, 337},
        };

        // Navigation keys (tab, escape, arrow, home/end)
        static constexpr std::array nav_key_ranges_{
            std::pair{262, 266},
            std::pair{268, 270},
            std::pair{256, 257},
            std::pair{258, 259},
        };

        std::array<int, 16> keys_queue_{};
        std::size_t         keys_queue_size_{};

        enum class move_direction : std::uint8_t {
            up,
            down,
            left,
            right,
        };

        [[nodiscard]]
        auto advance_focus_group_(bool reverse) const -> node *;

        [[nodiscard]]
        auto move_focus_in_group_(move_direction dir) const -> node *;

        auto gather_queued_keys_() -> void;

        [[nodiscard]]
        auto get_queued_keys_() const -> std::span<const int>;

        auto dispatch_nav_keys_() -> void;

        auto dispatch_keys_() const -> void;

        auto dispatch_deep_keys_() const -> void;

        auto dispatch_keyboard_cb_() const -> void;

        auto dispatch_keyboard_() -> void;

    public:
        // Lifecycle

        auto update() -> void;

        auto draw() const -> void;
    };

    BPP_GLOBAL ui_manager mgr;
} // namespace ui
