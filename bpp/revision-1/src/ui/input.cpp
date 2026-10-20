#include "input.hpp"

#include "manager.hpp"
#include "style.hpp"

bppui::button::button(const model &model) : node(model) {
    background_ = emplace<draw_box>();
    label_      = emplace<draw_text>();
    icon_       = emplace<draw_sprite>();
}

auto bppui::button::is_hoverable() const -> hoverable_type {
    return is_disabled ? hoverable_type::none : hoverable_type::capture;
}

auto bppui::button::on_mouse_enter() -> void {
    is_hovered_ = true;
}

auto bppui::button::on_mouse_leave() -> void {
    is_hovered_ = false;
}

auto bppui::button::on_mouse_press(const int button) -> void {
    if (button == MOUSE_BUTTON_LEFT) {
        is_mouse_held_ = true;
    }
}

auto bppui::button::on_mouse_release(const int button) -> void {
    if (button == MOUSE_BUTTON_LEFT) {
        is_mouse_held_ = false;
        if (hit_test(GetMousePosition())) {
            activated_ = true;
        }
    }
}

auto bppui::button::is_focusable() const -> focusable_type {
    return is_disabled ? focusable_type::none : focusable_type::focus;
}

auto bppui::button::on_focus_gained() -> void {
    is_focused_ = true;
}

auto bppui::button::on_focus_lost() -> void {
    is_focused_  = false;
    is_key_held_ = false;
}

auto bppui::button::on_key_pressed(const int key) -> void {
    if (key == KEY_ENTER || key == KEY_SPACE) {
        is_key_held_ = true;
    }
}

auto bppui::button::on_key_released(const int key) -> void {
    if (key == KEY_ENTER || key == KEY_SPACE) {
        if (is_key_held_) {
            activated_ = true;
        }

        is_key_held_ = false;
    }
}

auto bppui::button::on_update() -> void {
    if (activated_) {
        if (is_toggle) {
            toggled = !toggled;
            if (callback != nullptr) callback(toggled);
        } else {
            if (callback != nullptr) callback(activated_);
        }

        activated_ = false;
    }

    const style &style{mgr.get_style()};

    label_->text         = label;
    label_->font         = style.fonts.bold;
    label_->font_size    = style.medium_text.font_size;
    label_->spacing      = style.medium_text.spacing;
    label_->line_spacing = style.medium_text.line_spacing;
    label_->align        = {0.5F, 0.5F};
    label_->color        = style.foreground_color;

    style_element element{style.base};

    if ((is_toggle && toggled) || (!is_toggle && !is_secondary)) {
        element.background_color = style.accent_color;
        label_->color            = style.active_color;
    } else {
        element = element * style.bordered_scale + style.bordered_offset;
    }

    if (is_hovered_) {
        element = element * style.hover_scale + style.hover_offset;
    }

    if (is_mouse_held_ || is_key_held_) {
        element = element * style.held_scale + style.held_offset;
    }

    if (is_focused_) {
        element = element * style.focused_scale + style.focused_offset;
    }

    if (is_disabled) {
        element = element * style.disabled_scale + style.disabled_offset;
    }

    background_->color            = element.background_color;
    background_->border           = element.border_color;
    background_->border_thickness = element.border_thickness;
    background_->border_radius    = element.corner_radius;
    background_->shadow_color     = element.shadow_color;
    background_->shadow_radius    = element.shadow_radius;
    background_->shadow_shrink    = element.shadow_shrink;
    background_->shadow_offset    = element.shadow_offset;
}

bppui::checkbox::checkbox(const model &model) : node(model) {
    background_ = emplace<draw_box>();
    label_      = emplace<draw_text>();
    icon_       = emplace<draw_sprite>();
}

auto bppui::checkbox::is_hoverable() const -> hoverable_type {
    return is_disabled ? hoverable_type::none : hoverable_type::capture;
}

auto bppui::checkbox::on_mouse_enter() -> void {
    is_hovered_ = true;
}

auto bppui::checkbox::on_mouse_leave() -> void {
    is_hovered_ = false;
}

auto bppui::checkbox::on_mouse_press(const int button) -> void {
    if (button == MOUSE_BUTTON_LEFT) {
        is_mouse_held_ = true;
    }
}

auto bppui::checkbox::on_mouse_release(const int button) -> void {
    if (button == MOUSE_BUTTON_LEFT) {
        is_mouse_held_ = false;
        if (hit_test(GetMousePosition())) {
            activated_ = true;
        }
    }
}

auto bppui::checkbox::is_focusable() const -> focusable_type {
    return is_disabled ? focusable_type::none : focusable_type::focus;
}

auto bppui::checkbox::on_focus_gained() -> void {
    is_focused_ = true;
}

auto bppui::checkbox::on_focus_lost() -> void {
    is_focused_  = false;
    is_key_held_ = false;
}

auto bppui::checkbox::on_key_pressed(const int key) -> void {
    if (key == KEY_ENTER || key == KEY_SPACE) {
        is_key_held_ = true;
    }
}

auto bppui::checkbox::on_key_released(const int key) -> void {
    if (key == KEY_ENTER || key == KEY_SPACE) {
        if (is_key_held_) {
            activated_ = true;
        }

        is_key_held_ = false;
    }
}

auto bppui::checkbox::on_update() -> void {
    if (activated_) {
        toggled = !toggled;
        if (callback != nullptr) callback(toggled);

        activated_ = false;
    }

    const style &style{mgr.get_style()};

    label_->text         = label;
    label_->font         = style.fonts.bold;
    label_->font_size    = style.medium_text.font_size;
    label_->spacing      = style.medium_text.spacing;
    label_->line_spacing = style.medium_text.line_spacing;
    label_->align        = {0.5F, 0.5F};
    label_->color        = style.foreground_color;

    style_element element{style.base};

    if (toggled) {
        element.background_color = style.accent_color;
        label_->color            = style.active_color;
    } else {
        element = element * style.bordered_scale + style.bordered_offset;
    }

    if (is_hovered_) {
        element = element * style.hover_scale + style.hover_offset;
    }

    if (is_mouse_held_ || is_key_held_) {
        element = element * style.held_scale + style.held_offset;
    }

    if (is_focused_) {
        element = element * style.focused_scale + style.focused_offset;
    }

    if (is_disabled) {
        element = element * style.disabled_scale + style.disabled_offset;
    }

    background_->color            = element.background_color;
    background_->border           = element.border_color;
    background_->border_thickness = element.border_thickness;
    background_->border_radius    = element.corner_radius;
    background_->shadow_color     = element.shadow_color;
    background_->shadow_radius    = element.shadow_radius;
    background_->shadow_shrink    = element.shadow_shrink;
    background_->shadow_offset    = element.shadow_offset;
}
