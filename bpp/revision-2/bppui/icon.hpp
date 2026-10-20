#pragma once

#include <array>
#include <cstdint>

namespace bppui {
    struct icon_command {
        enum class command_type : std::uint8_t {
            line,
            circle,
            circle_outline,
            rectangle,
            rectangle_outline,
            triangle,
            triangle_outline,
            rounded_rectangle,
            rounded_rectangle_outline,
            ellipse,
            ellipse_outline,
        };

        command_type          command;
        std::array<float, 16> parameters;

        auto draw() const -> void;
    };
} // namespace ui
