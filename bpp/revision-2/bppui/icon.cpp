#include "icon.hpp"

#include "raylib.h"
#include "utils.hpp"

auto bppui::icon_command::draw() const -> void {
    const auto &p{parameters}; // NOLINT(*-identifier-length)

    switch (command) {
    case command_type::line: DrawLineEx({p[0], p[1]}, {p[2], p[3]}, p[4], color_from_hsl(p[5], p[6], p[7], p[8])); break;
    case command_type::circle: DrawCircleV({p[0], p[1]}, p[2], color_from_hsl(p[3], p[4], p[5], p[6])); break;
    case command_type::circle_outline: DrawRing({p[0], p[1]}, p[2] - p[3] / 2.0F, p[2] + p[3] / 2.0F, 0.0F, 0.0F, 0, color_from_hsl(p[4], p[5], p[6], p[7])); break;
    case command_type::rectangle: DrawRectangleRec({p[0], p[1], p[2], p[3]}, color_from_hsl(p[4], p[5], p[6], p[7])); break;
    case command_type::rectangle_outline: DrawRectangleLinesEx({p[0], p[1], p[2], p[3]}, p[4], color_from_hsl(p[5], p[6], p[7], p[8])); break;
    case command_type::triangle: DrawTriangle({p[0], p[1]}, {p[2], p[3]}, {p[4], p[5]}, color_from_hsl(p[6], p[7], p[8], p[9])); break;
    case command_type::triangle_outline:
        DrawLineEx({p[0], p[1]}, {p[2], p[3]}, p[6], color_from_hsl(p[7], p[8], p[9], p[10]));
        DrawLineEx({p[2], p[3]}, {p[4], p[5]}, p[6], color_from_hsl(p[7], p[8], p[9], p[10]));
        DrawLineEx({p[4], p[5]}, {p[0], p[1]}, p[6], color_from_hsl(p[7], p[8], p[9], p[10]));
        break;
    case command_type::rounded_rectangle: draw_rec_rounded({p[0], p[1], p[2], p[3]}, p[4], color_from_hsl(p[5], p[6], p[7], p[8])); break;
    case command_type::rounded_rectangle_outline: draw_rec_rounded_lines({p[0], p[1], p[2], p[3]}, p[4], p[5], color_from_hsl(p[6], p[7], p[8], p[9])); break;
    case command_type::ellipse: DrawEllipseV({p[0], p[1]}, p[2], p[3], color_from_hsl(p[4], p[5], p[6], p[7])); break;
    case command_type::ellipse_outline: DrawEllipseLinesV({p[0], p[1]}, p[2], p[3], color_from_hsl(p[4], p[5], p[6], p[7])); break;
    }
}
