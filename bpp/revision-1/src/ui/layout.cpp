#include "layout.hpp"

auto bppui::stack::on_model() -> void {
    model self_model = get_model();
    self_model.scale = {};

    if (get_children().empty()) {
        self_model.extent = {};
        set_model(self_model);
        return;
    }

    const Rectangle parent_bounds{get_parent_bounds()};

    float           cursor_main{padding};
    float           max_cross{};

    for (const auto &child : get_children()) {
        const Vector2 child_size{rec_size(child->get_model().compute(parent_bounds))};
        const float   child_main{vertical ? child_size.y : child_size.x};
        const float   child_cross{vertical ? child_size.x : child_size.y};

        model         child_model = child->get_model();
        child_model.relative      = true;
        child_model.offset        = vertical ? Vector2{padding, cursor_main} : Vector2{cursor_main, padding};
        child_model.anchor        = {};
        child_model.pivot         = {};
        child->set_model(child_model);

        cursor_main += child_main + gap;
        max_cross = std::max(max_cross, child_cross);
    }

    self_model.extent = vertical ? Vector2{max_cross + padding * 2.0F, cursor_main - gap + padding} : Vector2{cursor_main - gap + padding, max_cross + padding * 2.0F};
    set_model(self_model);
}

auto bppui::flow::on_model() -> void {
    model self_model = get_model();
    self_model.scale = {};

    if (get_children().empty() || length == 0) {
        self_model.extent = {};
        set_model(self_model);
        return;
    }

    const Rectangle parent_bounds{get_parent_bounds()};

    float           cursor_main{padding};
    float           cursor_cross{padding};
    float           max_line_cross_size{};
    float           max_main_size{};

    for (const auto &child : get_children()) {
        const Vector2 child_size{rec_size(child->get_model().compute(parent_bounds))};
        const float   child_main{vertical ? child_size.y : child_size.x};
        const float   child_cross{vertical ? child_size.x : child_size.y};

        if (cursor_main > padding && cursor_main + child_main >= length - padding) {
            cursor_cross += max_line_cross_size + gap;
            max_main_size       = std::max(max_main_size, cursor_main - gap + padding);

            cursor_main         = padding;
            max_line_cross_size = 0.0F;
        }

        model child_model    = child->get_model();
        child_model.relative = true;
        child_model.offset   = vertical ? Vector2{cursor_cross, cursor_main} : Vector2{cursor_main, cursor_cross};
        child_model.anchor   = {};
        child_model.pivot    = {};
        child->set_model(child_model);

        cursor_main += child_main + gap;
        max_line_cross_size = std::max(max_line_cross_size, child_cross);
    }

    max_main_size = std::max(max_main_size, cursor_main - gap + padding);
    const float max_cross_size{cursor_cross + max_line_cross_size + padding};

    self_model.extent = vertical ? Vector2{max_cross_size, max_main_size} : Vector2{max_main_size, max_cross_size};
    set_model(self_model);
}
