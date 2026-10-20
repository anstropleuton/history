#pragma once

#include "node.hpp"

namespace bppui {
    // Layout

    class stack final : public node {
    public:
        float gap{};
        bool  vertical{};

        explicit stack() = default;

        auto are_children_arranged() -> bool override {
            return true;
        }

        auto arrange() -> void override;
    };

    class flow final : public node {
    public:
        float   length{};
        Vector2 gap{};
        bool    vertical{};

        explicit flow() = default;

        auto are_children_arranged() -> bool override {
            return true;
        }

        auto arrange() -> void override;
    };
} // namespace bppui
