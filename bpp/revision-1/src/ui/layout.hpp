#pragma once

#include "node.hpp"

namespace bppui {
    // Layout

    class stack final : public node {
    public:
        float padding{};
        float gap{};
        bool  vertical{};

        explicit stack() = default;

        [[nodiscard]]
        auto is_bound_transparent() const -> bool override {
            return true;
        }

        auto on_model() -> void override;
    };

    class flow final : public node {
    public:
        float length{};
        float padding{};
        float gap{};
        bool  vertical{};

        explicit flow() = default;

        [[nodiscard]]
        auto is_bound_transparent() const -> bool override {
            return true;
        }

        auto on_model() -> void override;
    };
} // namespace ui
