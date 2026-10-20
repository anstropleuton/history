#include "display.hpp"

#include "manager.hpp"

bppui::panel::panel(const model &model) : node(model) {
    box_ = emplace<draw_box>();
}

auto bppui::panel::is_focusable() const -> focusable_type {
    return is_focus_group ? focusable_type::focus_group : focusable_type::none;
}

void bppui::panel::on_update() {
    const style  &style{mgr.get_style()};
    style_element element{style.base};
    element                = element * style.layered_scale + style.layered_offset;

    box_->color            = element.background_color;
    box_->border           = element.border_color;
    box_->border_thickness = element.border_thickness;
    box_->border_radius    = element.corner_radius;
    box_->shadow_color     = element.shadow_color;
    box_->shadow_radius    = element.shadow_radius;
    box_->shadow_shrink    = element.shadow_shrink;
    box_->shadow_offset    = element.shadow_offset;
}

bppui::label::label(const model &model) : node(model) {
    text_ = emplace<draw_text>();
}

void bppui::label::on_update() {
    const style &style{mgr.get_style()};

    text_->text  = text;
    text_->align = align;

    switch (variant) {
    case font_variant::regular: text_->font = style.fonts.regular; break;
    case font_variant::bold: text_->font = style.fonts.bold; break;
    case font_variant::italic: text_->font = style.fonts.italic; break;
    case font_variant::bold_italic: text_->font = style.fonts.bold_italic; break;
    }

    style_text text_style{};

    switch (size) {
    case text_size::small: text_style = style.small_text; break;
    case text_size::medium: text_style = style.medium_text; break;
    case text_size::large: text_style = style.large_text; break;
    }

    text_->font_size    = text_style.font_size;
    text_->spacing      = text_style.spacing;
    text_->line_spacing = text_style.line_spacing;
    text_->color        = style.foreground_color;
}
