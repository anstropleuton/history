#include "manager.hpp"

#include "raymath.h"

#include <algorithm>

bppui::ui_manager bppui::mgr;

inline auto bppui::ui_manager::set_hovered_(node *const new_node) -> void {
    if (hovered_ != nullptr) {
        hovered_->on_mouse_leave();
    }

    hovered_ = new_node;

    if (hovered_ != nullptr) {
        hovered_->on_mouse_enter();
    }
}

auto bppui::ui_manager::associated_focus_group_() const -> node * {
    node *focus_group{};

    node *current{focused_};
    while (current != nullptr) {
        if (current->is_focusable() == node::focusable_type::focus_group) {
            focus_group = current;
            break;
        }
        current = current->parent_;
    }

    return focus_group;
}

auto bppui::ui_manager::initial_focusable_in_group_(const bool prefer_last) const -> node * {
    constexpr std::size_t full_depth{std::numeric_limits<std::size_t>::max()};

    node                 *focusable{};
    node                 *current{focused_group_};
    while (current != nullptr) {
        const auto focus{current->is_focusable()};

        if (focus == node::focusable_type::focus_group) {
            current = current->preorder_next(focused_group_->depth_, current->depth_);
            continue;
        }

        if (focus == node::focusable_type::focus || focus == node::focusable_type::deep_focus) {
            focusable = current;
            if (!prefer_last) {
                break;
            }
        }

        current = current->preorder_next(focused_group_->depth_, full_depth);
    }

    return focusable;
}

auto bppui::ui_manager::restore_focus_state_() -> bool {
    bool restored{};
    if (focused_ == nullptr && recently_focused_ != nullptr) {
        set_focused_(recently_focused_);
        recently_focused_ = nullptr;
        restored          = true;
    }

    if (focused_group_ == nullptr && recently_focused_group_ != nullptr) {
        set_focused_group_(recently_focused_group_);
        recently_focused_group_ = nullptr;
        restored                = true;
    }
    return restored;
}

auto bppui::ui_manager::set_focused_(node *const new_focusable) -> void {
    if (focused_ == new_focusable) {
        return;
    }

    if (focused_ != nullptr) {
        focused_->on_focus_lost();
        recently_focused_ = focused_;
    }

    focused_ = new_focusable;

    if (focused_ != nullptr) {
        focused_->on_focus_gained();

        if (node *new_focus_group{associated_focus_group_()}; new_focus_group != focused_group_) {
            if (focused_group_ != nullptr) {
                focused_group_->on_focus_lost();
            }

            focused_group_ = new_focus_group;

            if (focused_group_ != nullptr) {
                focused_group_->on_focus_gained();
            }
        }
    } else {
        focused_group_ = nullptr;
    }
}

auto bppui::ui_manager::set_focused_group_(node *const new_focus_group) -> void {
    if (focused_group_ == new_focus_group) {
        return;
    }

    if (focused_group_ != nullptr) {
        focused_group_->on_focus_lost();
        recently_focused_group_ = focused_group_;
    }

    focused_group_ = new_focus_group;

    if (focused_group_ != nullptr) {
        focused_group_->on_focus_gained();

        if (node *new_focusable{initial_focusable_in_group_(false)}; new_focusable != focused_) {
            if (focused_ != nullptr) {
                focused_->on_focus_lost();
            }

            focused_ = new_focusable;

            if (focused_ != nullptr) {
                focused_->on_focus_gained();
            }
        }
    } else {
        focused_ = nullptr;
    }
}

auto bppui::ui_manager::hit_test_() const -> node * {
    node *found{};

    node *current{root_.get()};
    while (current != nullptr) {
        if (current->is_hoverable() == node::hoverable_type::none) {
            current = current->preorder_next();
            continue;
        }

        if (current->hit_test(GetMousePosition())) {
            found = current;
            break;
        }

        current = current->preorder_next();
    }

    return found;
}

auto bppui::ui_manager::dispatch_hover_() -> void {
    if (captured_ != nullptr) {
        return;
    }

    if (node *const current_hovered{hit_test_()}; current_hovered != hovered_) {
        set_hovered_(current_hovered);
    }
}

auto bppui::ui_manager::dispatch_capture_() -> void {
    if (captured_ != nullptr) {
        return;
    }

    if (hovered_ == nullptr || hovered_->is_hoverable() != node::hoverable_type::capture) {
        bool any_button_pressed{};
        for (int button{}; button < mouse_buttons_count; ++button) {
            if (IsMouseButtonPressed(button)) {
                any_button_pressed = true;
                break;
            }
        }
        if (any_button_pressed) {
            set_focused_(nullptr);
        }
        return;
    }

    int pressed_mouse_button{mouse_button_none};

    for (int button{}; button < mouse_buttons_count; ++button) {
        if (IsMouseButtonPressed(button)) {
            pressed_mouse_button = button;
            break;
        }
    }

    if (pressed_mouse_button != mouse_button_none) {
        captured_      = hovered_;
        captured_with_ = pressed_mouse_button;
        captured_->on_mouse_press(captured_with_);

        if (const auto focus{captured_->is_focusable()}; focus == node::focusable_type::focus || focus == node::focusable_type::deep_focus) {
            recently_focused_ = captured_;
            if (node *const focus_group{associated_focus_group_()}; focus_group != nullptr) {
                recently_focused_group_ = focus_group;
            }
            if (focused_ != nullptr) {
                set_focused_(recently_focused_);
            }
        }
    }
}

auto bppui::ui_manager::dispatch_uncapture_() -> void {
    if (captured_ == nullptr) {
        return;
    }

    if (IsMouseButtonReleased(captured_with_)) {
        captured_->on_mouse_release(captured_with_);
        captured_      = nullptr;
        captured_with_ = mouse_button_none;
    }
}

auto bppui::ui_manager::dispatch_mouse_move_() const -> void {
    const Vector2 delta{GetMouseDelta()};
    if (Vector2LengthSqr(delta) == 0.0F) {
        return;
    }

    const Vector2 prev{GetMousePosition() - delta};

    if (node *const mouse_into{
            captured_ != nullptr
                ? captured_
            : hovered_ != nullptr
                ? hovered_
                : nullptr};
        mouse_into != nullptr) {
        mouse_into->on_mouse_move(GetMousePosition(), prev, delta);
    } else {
        if (mouse_move_ != nullptr) {
            mouse_move_(GetMousePosition(), prev, delta);
        }
    }
}

auto bppui::ui_manager::dispatch_mouse_wheel_move_() const -> void {
    if (node *const mouse_into{
            captured_ != nullptr
                ? captured_
            : hovered_ != nullptr
                ? hovered_
                : nullptr};
        mouse_into != nullptr) {
        if (const Vector2 delta{GetMouseWheelMoveV()}; Vector2LengthSqr(delta) > 0.0F) {
            mouse_into->on_mouse_wheel_move(delta);
        }
    }
}

auto bppui::ui_manager::dispatch_mouse_cb_() const -> void {
    if (mouse_button_press_ != nullptr) {
        for (int button{}; button < mouse_buttons_count; ++button) {
            if (button != captured_with_ && IsMouseButtonPressed(button)) {
                mouse_button_press_(button);
            }
        }
    }

    if (mouse_button_release_ != nullptr) {
        for (int button{}; button < mouse_buttons_count; ++button) {
            if (button != captured_with_ && IsMouseButtonReleased(button)) {
                mouse_button_release_(button);
            }
        }
    }

    if (mouse_move_ != nullptr && hovered_ == nullptr) {
        const Vector2 pos{GetMousePosition()};
        const Vector2 delta{GetMouseDelta()};
        const Vector2 prev{pos - delta};
        mouse_move_(pos, prev, delta);
    }

    if (mouse_wheel_move_ != nullptr && hovered_ == nullptr) {
        mouse_wheel_move_(GetMouseWheelMoveV());
    }
}

auto bppui::ui_manager::dispatch_mouse_() -> void {
    dispatch_hover_();
    dispatch_capture_();
    dispatch_uncapture_();
    dispatch_mouse_move_();
    dispatch_mouse_wheel_move_();
    dispatch_mouse_cb_();
}

auto bppui::ui_manager::advance_focus_group_(const bool reverse) const -> node * {
    node *current{focused_group_ == nullptr ? root_.get() : focused_group_};
    while (true) {
        current = reverse ? current->preorder_prev() : current->preorder_next();
        if (current == nullptr) {
            break;
        }

        if (current->is_focusable() == node::focusable_type::focus_group) {
            return current;
        }
    }

    return nullptr;
}

auto bppui::ui_manager::move_focus_in_group_(const move_direction dir) const -> node * {
    if (focused_ == nullptr) {
        return nullptr;
    }

    Vector2         focused_center{rec_center(focused_->bounds_)};

    constexpr float forward_bias{1.0F};
    constexpr float side_bias{0.5F};

    switch (dir) {
    case move_direction::up:
        focused_center.y -= forward_bias;
        focused_center.x -= side_bias;
        break;

    case move_direction::down:
        focused_center.y += forward_bias;
        focused_center.x += side_bias;
        break;

    case move_direction::left:
        focused_center.x -= forward_bias;
        focused_center.y += side_bias;
        break;

    case move_direction::right:
        focused_center.x += forward_bias;
        focused_center.y -= side_bias;
        break;
    }

    node *best{nullptr};
    float best_score{std::numeric_limits<float>::max()};

    for (node *current{initial_focusable_in_group_(false)}; current != nullptr; current = current->preorder_next()) {
        if (current == focused_) {
            continue;
        }

        if (const auto focus{current->is_focusable()}; focus != node::focusable_type::focus && focus != node::focusable_type::deep_focus) {
            continue;
        }

        const Vector2 current_center{rec_center(current->bounds_)};
        const auto [dx, dy]{current_center - focused_center};

        switch (dir) {
        case move_direction::up:
            if (dy > 0.0F) {
                continue;
            }
            break;
        case move_direction::down:
            if (dy < 0.0F) {
                continue;
            }
            break;
        case move_direction::left:
            if (dx > 0.0F) {
                continue;
            }
            break;
        case move_direction::right:
            if (dx < 0.0F) {
                continue;
            }
            break;
        }

        float score{};
        if (dir == move_direction::up || dir == move_direction::down) {
            score = dy * dy + 4.0F * dx * dx;
        } else {
            score = 4.0F * dy * dy + dx * dx;
        }

        if (score < best_score) {
            best_score = score;
            best       = current;
        }

        current = current->preorder_next();
    }

    return best;
}

auto bppui::ui_manager::gather_queued_keys_() -> void {
    int character{};
    keys_queue_size_ = 0;
    while ((character = GetCharPressed()) != 0) {
        keys_queue_.at(keys_queue_size_++) = character;
    }
}

auto bppui::ui_manager::get_queued_keys_() const -> std::span<const int> {
    return {keys_queue_.data(), keys_queue_size_};
}

auto bppui::ui_manager::dispatch_nav_keys_() -> void {
    // ReSharper disable once CppDFANullDereference
    if (focused_ != nullptr && focused_->is_focusable() == node::focusable_type::deep_focus) {
        return;
    }

    const bool key_tab{is_key_combo_pressed(hotkey::none, KEY_TAB)};        // Move to next focus group and move focus to first in focus group
    const bool key_shift_tab{is_key_combo_pressed(hotkey::shift, KEY_TAB)}; // Move to previous focus group and move focus to first in focus group
    const bool key_escape{is_key_combo_pressed(hotkey::none, KEY_ESCAPE)};  // Remove focus
    const bool key_up{is_key_combo_pressed(hotkey::none, KEY_UP)};          // Move to focusable node in group towards top of currently focused node
    const bool key_down{is_key_combo_pressed(hotkey::none, KEY_DOWN)};      // Move to focusable node in group towards bottom of currently focused node
    const bool key_left{is_key_combo_pressed(hotkey::none, KEY_LEFT)};      // Move to focusable node in group towards left of currently focused node
    const bool key_right{is_key_combo_pressed(hotkey::none, KEY_RIGHT)};    // Move to focusable node in group towards right of currently focused node
    const bool key_home{is_key_combo_pressed(hotkey::none, KEY_HOME)};      // Move to first focusable node in group
    const bool key_end{is_key_combo_pressed(hotkey::none, KEY_END)};        // Move to last focusable node in group

    if (!key_tab && !key_shift_tab && !key_escape && !key_up && !key_down && !key_left && !key_right && !key_home && !key_end) {
        return;
    }

    if (restore_focus_state_()) {
        return;
    }

    if (key_tab) {
        if (node *const advanced_focused{advance_focus_group_(false)}; advanced_focused != nullptr) {
            set_focused_group_(advanced_focused);
            return;
        }
    } else if (key_shift_tab) {
        if (node *const advanced_focused{advance_focus_group_(true)}; advanced_focused != nullptr) {
            set_focused_group_(advanced_focused);
            return;
        }
    } else if (key_escape) {
        set_focused_(nullptr);
        return;
    } else if (key_home) {
        if (node *const initial_focusable{initial_focusable_in_group_(false)}; initial_focusable != nullptr) {
            set_focused_(initial_focusable);
            return;
        }
    } else if (key_end) {
        if (node *const initial_focusable{initial_focusable_in_group_(true)}; initial_focusable != nullptr) {
            set_focused_(initial_focusable);
            return;
        }
    }

    if (!key_up && !key_down && !key_left && !key_right) {
        return;
    }

    const move_direction dir{
        key_up
            ? move_direction::up
        : key_down
            ? move_direction::down
        : key_left
            ? move_direction::left
            : move_direction::right,
    };

    if (node *const moved_focused{move_focus_in_group_(dir)}; moved_focused != nullptr) {
        set_focused_(moved_focused);
    } else if (focused_ == nullptr && recently_focused_ == nullptr) {
        set_focused_group_(advance_focus_group_(false));
    }
}

auto bppui::ui_manager::dispatch_keys_() const -> void {
    if (focused_ == nullptr) {
        return;
    }

    for (const auto &[first, last] : key_ranges_) {
        for (int key{first}; key < last; ++key) {
            if (IsKeyPressed(key)) {
                focused_->on_key_pressed(key);
            }
            if (IsKeyReleased(key)) {
                focused_->on_key_released(key);
            }
        }
    }
}

auto bppui::ui_manager::dispatch_deep_keys_() const -> void {
    if (focused_ == nullptr) {
        return;
    }

    if (focused_->is_focusable() == node::focusable_type::deep_focus) {
        for (const auto &[first, last] : nav_key_ranges_) {
            for (int key{first}; key < last; ++key) {
                if (IsKeyPressed(key)) {
                    focused_->on_key_pressed(key);
                }
                if (IsKeyReleased(key)) {
                    focused_->on_key_released(key);
                }
            }
        }

        focused_->on_key_text(get_queued_keys_());
    }
}

auto bppui::ui_manager::dispatch_keyboard_cb_() const -> void {
    if (focused_ == nullptr) {
        return;
    }

    if (const auto focus{focused_->is_focusable()}; focus == node::focusable_type::deep_focus) {
        return;
    }

    for (const auto &[first, last] : key_ranges_) {
        for (int key{first}; key < last; ++key) {
            if (keyboard_key_press_ != nullptr && IsKeyPressed(key)) {
                keyboard_key_press_(key);
            }
            if (keyboard_key_release_ != nullptr && IsKeyReleased(key)) {
                keyboard_key_release_(key);
            }
        }
    }

    if (keyboard_key_text_ != nullptr) {
        keyboard_key_text_(get_queued_keys_());
    }
}

auto bppui::ui_manager::dispatch_keyboard_() -> void {
    gather_queued_keys_();
    dispatch_nav_keys_();
    dispatch_keys_();
    dispatch_deep_keys_();
    dispatch_keyboard_cb_();
}

auto bppui::ui_manager::update() -> void {
    if (IsWindowResized()) {
        root_->dirty_ = true;
    }

    {
        node *current{root_.get()};
        while (current != nullptr) {
            if (current->parent_ != nullptr && current->parent_->dirty_) {
                current->dirty_ = true;
            }
            current = current->preorder_next();
        }
    }

    {
        node *current{root_.get()};
        while (current != nullptr) {
            if (current->dirty_) {
                current->on_model();
                const Rectangle parent_bounds{current->get_parent_bounds()};
                current->bounds_ = current->model_.compute(parent_bounds);
            }
            current = current->preorder_next();
        }
    }

    {
        node *current{root_.get()};
        while (current != nullptr) {
            if (current->dirty_) {
                current->on_layout();
                current->dirty_ = false;
            }
            current = current->preorder_next();
        }
    }

    dispatch_mouse_();
    dispatch_keyboard_();

    {
        node *current{root_.get()};
        while (current != nullptr) {
            current->on_update();
            current = current->preorder_next();
        }
    }
}

auto bppui::ui_manager::draw() const -> void {
    {
        node *current{root_.get()};
        while (current != nullptr) {
            current->on_underlay();
            current = current->preorder_next();
        }
    }

    {
        node *current{root_.get()};
        while (current != nullptr) {
            current->on_draw();
            current = current->preorder_next();
        }
    }

    {
        node *current{root_.get()};
        while (current != nullptr) {
            current->on_overlay();
            current = current->preorder_next();
        }
    }

    if (debug_draw_) {
        node *current{root_.get()};
        while (current != nullptr) {
            current->on_debug_draw();
            current = current->preorder_next();
        }
    }
}
