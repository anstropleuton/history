/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Implementations for layouts in @c minui.hpp .
 *
 *  @copyright  Copyright (c) 2024 Anstro Pleuton
 *
 *   __  __ _             _
 *  |  \/  (_)_ __  _   _(_)
 *  | |\/| | | '_ \| | | | |
 *  | |  | | | | | | |_| | |
 *  |_|  |_|_|_| |_|\__,_|_|
 *
 *  Minui is a GUI framework for Minesweeper.
 *
 *  This software is licensed under the terms of MIT License.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  Credits where credit's due:
 *  - ASCII Art generated using https://www.patorjk.com/software/taag with font
 *    "Standard".
 *  - Textures taken from Leyan Lo,
 *    https://github.com/leyanlo/minesweeper/tree/main/src/images
 *  - Windows 95 style pixel font taken from Alina Sava,
 *    https://fontsarena.com/w95fa-by-alina-sava
 */

#include <limits>
#include <ranges>
#include <vector>

#include "minui.hpp"
#include "raylib.h"

/**
 *  @brief   Calculate linear layout result.
 *
 *  @param   layout_pos   Layout's main-axis position.
 *  @param   layout_size  Layout's main-axis size.
 *  @param   l_bounds     Layout's bounds.
 *  @param   rules        Linear layout rules.
 *  @return  Linear layout result.
 */
[[nodiscard]] auto minui::calculate_linear_layout(
    float layout_pos, float layout_size, Rectangle l_bounds,
    std::vector<linear_rule> &rules
) -> linear_layout_result
{
    float epsilon = std::numeric_limits<float>::epsilon();
    linear_layout_result result = {};

    result.new_pos  = layout_pos;
    result.new_size = layout_size;

    // Step 0: Setups
    std::vector<linear_rule::eval> evals;
    evals.reserve(rules.size());
    float total_weight = 0.0f;

    for (auto &rule : rules)
    {
        auto eval = rule.evaluate(l_bounds);
        evals.emplace_back(eval);

        total_weight += eval.weight;
    }

    // Step 1: Minimum size as default
    result.rules.reserve(rules.size());
    float total_min_size = 0.0f;
    for (auto &eval : evals)
    {
        linear_layout_result::linear_rule_result rule = {};
        rule.size = result.new_size - eval.min_shrink;
        result.rules.emplace_back(rule);

        total_min_size += rule.size;
    }

    // Step 2: Layout expansion
    float remaining = result.new_size - total_min_size;
    if (remaining < epsilon)
    {
        result.new_pos  += remaining / 2.0f;
        result.new_size -= remaining;

        remaining = 0.0f;
    }

    // Step 3: Size distribution
    auto adjustables = std::views::iota(0zu, rules.size())
                     | std::ranges::to<std::vector<std::size_t>>();
    while (remaining > epsilon && total_weight > epsilon)
    {
        float distributed = 0.0f;

        std::vector<std::size_t> new_adjustables = {};
        for (std::size_t i : adjustables)
        {
            auto &rule = result.rules[i];
            auto &eval = evals[i];

            // Skip fixed rules
            if (eval.weight < epsilon)
            {
                continue;
            }

            // Skip already maxed rules
            float max_size = result.new_size - eval.max_shrink;
            if (rule.size >= max_size)
            {
                continue;
            }

            float allocate = eval.weight / total_weight * remaining;

            // Clamp to max
            if (rule.size + allocate > max_size)
            {
                allocate      = max_size - rule.size;
                total_weight -= eval.weight / allocate; // Remove clamped from weight
            }
            else
            {
                new_adjustables.emplace_back(i);
            }

            rule.size   += allocate;
            distributed += allocate;
        }

        if (distributed < epsilon)
        {
            break;
        }

        // Continue reducing to atoms
        remaining -= distributed;
        adjustables = std::move(new_adjustables);
    }

    // Step 4: Positioning
    float total_size = 0.0f;
    for (auto &rule : result.rules)
    {
        total_size += rule.size;
    }

    float begin   = result.new_pos + (result.new_size - total_size) / 2.0f;
    float current = begin;
    for (auto &rule : result.rules)
    {
        rule.pos = current;
        current += rule.size;
    }

    return result;
}

/**
 *  @brief  Layout rebounds before children.
 */
auto minui::column_layout::pre_rebound() -> void
{
    fit_parent_size();

    std::vector<std::shared_ptr<linear_item>> items;
    std::vector<linear_rule> rules;

    items.reserve(children.size());
    rules.reserve(children.size());

    for (auto &child : children)
    {
        if (auto item = std::dynamic_pointer_cast<linear_item>(child))
        {
            items.emplace_back(item);
            rules.emplace_back(item->rule);
        }
    }

    auto result = calculate_linear_layout(bounds.y, bounds.height,
        bounds, rules);

    bounds.y      = result.new_pos;
    bounds.height = result.new_size;

    for (std::size_t i = 0; i < items.size(); i++)
    {
        auto &item = items[i];
        auto &rule = result.rules[i];

        item->bounds.x      = bounds.x;
        item->bounds.y      = rule.pos;
        item->bounds.width  = bounds.width;
        item->bounds.height = rule.size;
    }
}

/**
 *  @brief  Layout rebounds before children.
 */
auto minui::row_layout::pre_rebound() -> void
{
    fit_parent_size();

    std::vector<std::shared_ptr<linear_item>> items;
    std::vector<linear_rule> rules;

    items.reserve(children.size());
    rules.reserve(children.size());

    for (auto &child : children)
    {
        if (auto item = std::dynamic_pointer_cast<linear_item>(child))
        {
            items.emplace_back(item);
            rules.emplace_back(item->rule);
        }
    }

    auto result = calculate_linear_layout(bounds.x, bounds.width,
        bounds, rules);

    bounds.x     = result.new_pos;
    bounds.width = result.new_size;

    for (std::size_t i = 0; i < items.size(); i++)
    {
        auto &item = items[i];
        auto &rule = result.rules[i];

        item->bounds.x      = rule.pos;
        item->bounds.y      = bounds.y;
        item->bounds.width  = rule.size;
        item->bounds.height = bounds.height;
    }
}

/**
 *  @brief  Layout rebounds before children.
 */
auto minui::grid_layout::pre_rebound() -> void
{
    fit_parent_size();

    // Evaluate rule
    std::vector<linear_rule::eval> row_consts;
    row_consts.reserve(rows.size());
    for (auto &row : rows)
    {
        row_consts.emplace_back(row.evaluate(bounds));
    }

    std::vector<linear_rule::eval> column_consts;
    column_consts.reserve(columns.size());
    for (auto &column : columns)
    {
        column_consts.emplace_back(column.evaluate(bounds));
    }
}

/**
 *  @brief  Layout updates before children.
 */
auto minui::int_layout::pre_update() -> void
{}

/**
 *  @brief  Layout updates before children.
 */
auto minui::int_layout::post_update() -> void
{}
