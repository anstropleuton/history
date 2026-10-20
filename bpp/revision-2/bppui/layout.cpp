#include "layout.hpp"

void bppui::stack::arrange() {
    float cursor_main{};

    for (const auto &child : get_children()) {
        const Vector2  child_size{rec_size(child->get_outer_bounds())};
        const float    child_main{vertical ? child_size.y : child_size.x};

        position_model pos_model{};
        pos_model.relative = true;
        pos_model.offset   = vertical ? Vector2{0.0F, cursor_main} : Vector2{cursor_main, 0.0F};
        pos_model.anchor   = {};
        pos_model.pivot    = {};
        child->set_position_unchecked(pos_model);

        cursor_main += child_main + gap;
    }
}

void bppui::flow::arrange() {
    const float gap_main{vertical ? gap.y : gap.x};
    const float gap_cross{vertical ? gap.x : gap.y};

    float       cursor_main{};
    float       cursor_cross{};
    float       max_line_cross_size{};

    for (const auto &child : get_children()) {
        const Vector2 child_size{rec_size(child->get_outer_bounds())};
        const float   child_main{vertical ? child_size.y : child_size.x};
        const float   child_cross{vertical ? child_size.x : child_size.y};

        if (cursor_main > 0.0F && cursor_main + child_main > length) {
            cursor_cross += max_line_cross_size + gap_cross;

            cursor_main         = 0.0F;
            max_line_cross_size = 0.0F;
        }

        position_model pos_model{};
        pos_model.relative = true;
        pos_model.offset   = vertical ? Vector2{cursor_cross, cursor_main} : Vector2{cursor_main, cursor_cross};
        pos_model.anchor   = {};
        pos_model.pivot    = {};
        child->set_position_unchecked(pos_model);

        cursor_main += child_main + gap_main;
        max_line_cross_size = std::max(max_line_cross_size, child_cross);
    }
}