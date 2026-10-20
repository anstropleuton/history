#pragma once

#include "raylib.h"
#include "utils.hpp"

#include <functional>
#include <limits>
#include <memory>
#include <span>
#include <stdexcept>

namespace bppui {
    enum class size_mode : std::uint8_t {
        fit,
        fill,
        fixed
    };

    struct size_model {
        size_mode mode{size_mode::fit};
        Vector2   fixed_size{};
        Vector4   margin{};  // Top, bottom, left, right
        Vector4   padding{}; // Top, bottom, left, right
    };

    // Only applicable if current size model is fixed
    struct position_model {
        bool    relative{true};
        Vector2 offset{};
        Vector2 anchor{0.5F}; // Unused if parent is fit
        Vector2 pivot{0.5F};
    };

    class ui_manager;

    class node {
    public:
        explicit node()                        = default;

        virtual ~node()                        = default;

        node(const node &)                     = delete;
        node(node &&)                          = delete;

        auto operator=(const node &) -> node & = delete;
        auto operator=(node &&) -> node &      = delete;

        // Dimensions
    private:
        position_model position_;
        size_model     size_;
        bool           dirty_{true};
        Rectangle      bounds_{};
        Rectangle      outer_bounds_{};
        Rectangle      inner_bounds_{};

    public:
        [[nodiscard]]
        auto get_bounds() const -> Rectangle {
            if (dirty_) return {};
            return bounds_;
        }

        [[nodiscard]]
        auto get_outer_bounds() const -> Rectangle {
            if (dirty_) return {};
            return outer_bounds_;
        }

        [[nodiscard]]
        auto get_inner_bounds() const -> Rectangle {
            if (dirty_) return {};
            return inner_bounds_;
        }

        [[nodiscard]]
        auto get_position_model() const -> position_model {
            return position_;
        }

        auto set_position_model(const position_model &position) -> void {
            if (parent_ != nullptr && parent_->are_children_arranged()) {
                return;
            }

            position_ = position;
            dirty_    = true;
        }

        [[nodiscard]]
        auto get_size_model() const -> size_model {
            return size_;
        }

        auto set_size_model(const size_model &size) -> void {
            if (is_self_measured()) {
                return;
            }

            size_  = size;
            dirty_ = true;
        }

        // For access by inherited methods
        auto set_position_unchecked(const position_model &position) -> void {
            position_ = position;
            dirty_    = true;
        }

        // For access by inherited methods
        auto set_size_unchecked(const size_model &size) -> void {
            size_  = size;
            dirty_ = true;
        }

        [[nodiscard]]
        auto get_parent_area() const -> Rectangle {
            return parent_ != nullptr ? parent_->inner_bounds_ : screen_rec();
        }

        [[nodiscard]]
        auto get_children_area() const -> Rectangle {
            Rectangle base{};
            for (const auto &child : children_) {
                base = union_rec(base, child->outer_bounds_);
            }
            return base;
        }

        virtual auto is_self_measured() -> bool {
            return false;
        }

        virtual auto are_children_arranged() -> bool {
            return false;
        }

        virtual auto measure() -> void {
        }

        virtual auto arrange() -> void {
        }

        // Hierarchy
    private:
        node                              *parent_{};
        std::size_t                        depth_{};
        node                              *prev_sibling_{};
        node                              *next_sibling_{};
        std::size_t                        sibling_index_{};
        std::vector<std::unique_ptr<node>> children_;

        auto update_depth_() -> void;

    public:
        [[nodiscard]]
        auto get_parent() const -> node * {
            return parent_;
        }

        [[nodiscard]]
        auto get_depth() const -> std::size_t {
            return depth_;
        }

        [[nodiscard]]
        auto get_children() const -> std::span<const std::unique_ptr<node>> {
            return children_;
        }

        [[nodiscard]]
        auto get_prev_sibling() const -> node * {
            return prev_sibling_;
        }

        [[nodiscard]]
        auto get_next_sibling() const -> node * {
            return next_sibling_;
        }

        [[nodiscard]]
        auto get_sibling_index() const -> std::size_t {
            return sibling_index_;
        }

        [[nodiscard]]
        auto is_root() const -> bool {
            return parent_ == nullptr;
        }

        [[nodiscard]]
        auto is_leaf() const -> bool {
            return children_.empty();
        }

        [[nodiscard]]
        auto is_ancestor_of(const node *descendant) const -> bool;

        auto insert(std::unique_ptr<node> child, std::size_t index) -> node *;

        auto append(std::unique_ptr<node> child) -> node *;

        template <typename node_type, typename... args_type>
            requires std::is_base_of_v<node, node_type>
        auto emplace(args_type &&...args) -> node_type * {
            return static_cast<node_type *>(append(std::make_unique<node_type>(std::forward<args_type>(args)...)));
        }

        [[nodiscard]]
        auto detach() -> std::unique_ptr<node>;

        auto reparent(node *const new_parent) -> void {
            if (new_parent == nullptr) {
                throw std::invalid_argument("parent is nullptr");
            }

            if (new_parent == this) {
                throw std::invalid_argument("cannot reparent to itself");
            }

            if (is_ancestor_of(new_parent)) {
                throw std::invalid_argument("cycle detected");
            }

            auto self{detach()};
            new_parent->append(std::move(self));
        }

    private:
        template <typename self_type>
        static auto descend_or_advance_(const self_type self, const bool reverse, const std::size_t min_depth, const std::size_t max_depth) -> self_type {
            if (!self->children_.empty()) {
                if (const self_type child{reverse ? self->children_.back().get() : self->children_.front().get()}; child->depth_ >= min_depth && child->depth_ <= max_depth) {
                    return child;
                }
            }

            for (self_type current{self}; current != nullptr; current = current->parent_) {
                if (const self_type sibling{reverse ? current->prev_sibling_ : current->next_sibling_}; sibling != nullptr) {
                    if (sibling->depth_ >= min_depth && sibling->depth_ <= max_depth) {
                        return sibling;
                    }
                }
            }

            return nullptr;
        }

        template <typename self_type>
        static auto advance_or_ascend_(const self_type self, const bool reverse, const std::size_t min_depth, const std::size_t max_depth) -> self_type {
            const self_type sibling{reverse ? self->prev_sibling_ : self->next_sibling_};
            self_type       current{sibling};

            if (current == nullptr) {
                return self->parent_ != nullptr && self->parent_->depth_ >= min_depth && self->parent_->depth_ <= max_depth ? self->parent_ : nullptr;
            }

            while (!current->children_.empty() && current->depth_ < max_depth) {
                current = reverse ? current->children_.back().get() : current->children_.front().get();
            }

            return current->depth_ >= min_depth && current->depth_ <= max_depth ? current : nullptr;
        }

    public:
        [[nodiscard]]
        auto preorder_next(const std::size_t min_depth = 0, const std::size_t max_depth = std::numeric_limits<std::size_t>::max()) -> node * {
            return descend_or_advance_(this, false, min_depth, max_depth);
        }

        [[nodiscard]]
        auto preorder_next(const std::size_t min_depth = 0, const std::size_t max_depth = std::numeric_limits<std::size_t>::max()) const -> const node * {
            return descend_or_advance_(this, false, min_depth, max_depth);
        }

        [[nodiscard]]
        auto preorder_prev(const std::size_t min_depth = 0, const std::size_t max_depth = std::numeric_limits<std::size_t>::max()) -> node * {
            return advance_or_ascend_(this, false, min_depth, max_depth);
        }

        [[nodiscard]]
        auto preorder_prev(const std::size_t min_depth = 0, const std::size_t max_depth = std::numeric_limits<std::size_t>::max()) const -> const node * {
            return advance_or_ascend_(this, false, min_depth, max_depth);
        }

        [[nodiscard]]
        auto postorder_next(const std::size_t min_depth = 0, const std::size_t max_depth = std::numeric_limits<std::size_t>::max()) -> node * {
            return advance_or_ascend_(this, true, min_depth, max_depth);
        }

        [[nodiscard]]
        auto postorder_next(const std::size_t min_depth = 0, const std::size_t max_depth = std::numeric_limits<std::size_t>::max()) const -> const node * {
            return advance_or_ascend_(this, true, min_depth, max_depth);
        }

        [[nodiscard]]
        auto postorder_prev(const std::size_t min_depth = 0, const std::size_t max_depth = std::numeric_limits<std::size_t>::max()) -> node * {
            return descend_or_advance_(this, true, min_depth, max_depth);
        }

        [[nodiscard]]
        auto postorder_prev(const std::size_t min_depth = 0, const std::size_t max_depth = std::numeric_limits<std::size_t>::max()) const -> const node * {
            return descend_or_advance_(this, true, min_depth, max_depth);
        }

        // Mouse events

        enum class hoverable_type : std::uint8_t {
            none,
            hover,
            capture
        };

        [[nodiscard]]
        virtual auto get_hoverable_type() const -> hoverable_type {
            return hoverable_type::none;
        }

        [[nodiscard]]
        virtual auto hit_test(const Vector2 point) const -> bool {
            return CheckCollisionPointRec(point, bounds_);
        }

        virtual auto on_mouse_enter() -> void {
        }

        virtual auto on_mouse_leave() -> void {
        }

        virtual auto on_mouse_press(const int button) -> void {
            std::ignore = button;
        }

        virtual auto on_mouse_release(const int button) -> void {
            std::ignore = button;
        }

        virtual auto on_mouse_move(const Vector2 pos, const Vector2 prev, const Vector2 delta) -> void {
            std::ignore = pos;
            std::ignore = prev;
            std::ignore = delta;
        }

        virtual auto on_mouse_wheel_move(const Vector2 delta) -> void {
            std::ignore = delta;
        }

        // Keyboard events

        enum class focusable_type : std::uint8_t {
            none,
            focus,      // Keyboard events captured
            deep_focus, // Captures navigation keys as well
            focus_group // Jumped from tab keys
        };

        [[nodiscard]]
        virtual auto get_focusable_type() const -> focusable_type {
            return focusable_type::none;
        }

        virtual auto on_focus_gained() -> void {
        }

        virtual auto on_focus_lost() -> void {
        }

        virtual auto on_key_pressed(const int key) -> void {
            std::ignore = key;
        }

        virtual auto on_key_released(const int key) -> void {
            std::ignore = key;
        }

        virtual auto on_key_text(const std::span<const int> chars) -> void {
            std::ignore = chars;
        }

        // Phase passes

        virtual auto on_update() -> void {
        }

        virtual auto on_underlay() const -> void {
        }

        virtual auto on_draw() const -> void {
        }

        virtual auto on_overlay() const -> void {
        }

        virtual auto on_debug_draw() -> void {
            const Color color{color_from_hsl(static_cast<float>(reinterpret_cast<std::uintptr_t>(this) % 36) * 10.0F, 1.0F, 0.5F)};
            DrawRectangleRec(bounds_, ColorAlpha(color, 0.1F));
            DrawRectangleLinesEx(bounds_, 1.0F, ColorAlpha(color, 0.5F));
        }

    private:
        friend class ui_manager;
    };

    template <typename node_type, typename... args_type>
        requires std::is_base_of_v<node, node_type>
    auto make(args_type &&...args) -> std::unique_ptr<node_type> {
        return std::make_unique<node_type>(std::forward<args_type>(args)...);
    }

    class focus_group final : public node {
    public:
        using node::node;

        [[nodiscard]]
        auto get_focusable_type() const -> focusable_type override {
            return focusable_type::focus_group;
        }
    };
} // namespace bppui
