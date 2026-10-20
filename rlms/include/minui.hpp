/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Main header file for the GUI framework.
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
 */

#pragma once

#include <memory>
#include <print>
#include <vector>

#include "flux.hpp"
#include "raylib.h"
#include "raymath.h"

#ifndef MINUI_EXTERN
/**
 *  @brief  Define this to define global variables.
 */
#define MINUI_EXTERN extern
#endif // ifndef MINUI_EXTERN

/**
 *  @brief  All Minui's contents in this namespace.
 */
namespace minui {

// GUI layout system

/**
 *  @brief  Draw debug stuff on screen (such as outline for layout).
 */
MINUI_EXTERN bool draw_debug;

/**
 *  @brief  Use bounds itself as view when updating (i.e., updatable area).
 */
MINUI_EXTERN bool unbounded_update;

/**
 *  @brief  Use bounds itself as view when drawing (i.e., drawable area).
 */
MINUI_EXTERN bool unbounded_draw;

/**
 *  @brief  Basic GUI layout node.
 */
struct layout {

    /**
     *  @brief  Parent layout for reference.
     */
    std::weak_ptr<layout> parent;

    /**
     *  @brief  Children layout.
     */
    std::vector<std::shared_ptr<layout>> children;

    /**
     *  @brief  Layout bounds.
     */
    Rectangle bounds = { 0.0f, 0.0f, 0.0f, 0.0f };

    /**
     *  @brief  Layout updatable/drawable area.
     *
     *  The view is the area which is effectively updatable or drawable.  If
     *  this layout has a parent layout whose view intersects with this layout's
     *  view, this layout's view is effectively "chopped" (Child - Parent), and
     *  the result is this view.  If you want to update and draw based on some
     *  "rectangle", this is the rectangle to use.
     */
    Rectangle view;

    /**
     *  @brief  Default constructor.
     */
    layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~layout() = default;

    /**
     *  @brief  Crop bound and parent's view as view.
     */
    auto refresh_view()
    {
        view = bounds;

        if (auto locked = parent.lock())
        {
            view = GetCollisionRec(view, locked->view);
        }
    }

    /**
     *  @brief   Get updatable bounds.  Use this to perform layout updates.
     *  @return  Updatable bounds.
     */
    [[nodiscard]] inline constexpr auto updatable_bounds() const
    {
        return unbounded_update ? bounds : view;
    }

    /**
     *  @brief   Get drawable bounds.  Use this to perform layout draws.
     *  @return  Drawable bounds.
     */
    [[nodiscard]] inline constexpr auto drawable_bounds() const
    {
        return unbounded_update ? bounds : view;
    }

    /**
     *  @brief  Match current size as parent's size.
     */
    auto fit_parent_size()
    {
        if (auto locked = parent.lock())
        {
            bounds = locked->bounds;
        }
    }

    /**
     *  @brief  Draw rectangle lines for bounds when @c draw_debug is set.
     */
    auto draw_debug_bounds() const
    {
        if (draw_debug)
        {
            DrawRectangleLinesEx(drawable_bounds(), 1.0f, GRAY);
        }
    }

    /**
     *  @brief  Layout rebounds before children.
     *
     *  Override this method if you want to alter the behavior of changing the
     *  bounds of the rectangle.
     *
     *  @note   Override this method if you want to do so after the children.
     *  @note   If you overriden pre_*, you should also override post_*.
     */
    virtual auto pre_rebound() -> void
    {
        fit_parent_size();
    }

    /**
     *  @brief  Layout rebounds after children.
     *
     *  Override this method if you want to alter the behavior of changing the
     *  bounds of the rectangle.
     *
     *  @note   Override this method if you want to do so after the children.
     *  @note   If you overriden post_*, you should also override pre_*.
     */
    virtual auto post_rebound() -> void
    {}

    /**
     *  @brief  Layout reviews before children.
     *
     *  Override this method if you want to alter the behavior of changing the
     *  views of the rectangle.
     *
     *  @note   Override this method if you want to do so before the children.
     *  @note   If you overriden pre_*, you should also override post_*.
     */
    virtual auto pre_review() -> void
    {
        refresh_view();
    }

    /**
     *  @brief  Layout reviews after children.
     *
     *  Override this method if you want to alter the behavior of changing the
     *  views of the rectangle.
     *
     *  @note   Override this method if you want to do so after the children.
     *  @note   If you overriden pre_*, you should also override post_*.
     */
    virtual auto post_review() -> void
    {}

    /**
     *  @brief  Layout updates before children.
     *
     *  Override this method if you want to add custom update logic.
     *
     *  @note   Override this method if you want to do so before the children.
     *  @note   If you overriden pre_*, you should also override post_*.
     */
    virtual auto pre_update() -> void
    {}

    /**
     *  @brief  Layout updates after children.
     *
     *  Override this method if you want to add custom update logic.
     *
     *  @note   Override this method if you want to do so after the children.
     *  @note   If you overriden pre_*, you should also override post_*.
     */
    virtual auto post_update() -> void
    {}

    /**
     *  @brief  Layout draws before children.
     *
     *  Override this method if you want to add custom draw logic.
     *
     *  @note   Override this method if you want to do so before the children.
     *  @note   If you overriden pre_*, you should also override post_*.
     */
    virtual auto pre_draw() const -> void
    {
        draw_debug_bounds();
    }

    /**
     *  @brief  Layout draws after children.
     *
     *  Override this method if you want to add custom draw logic.
     *
     *  @note   Override this method if you want to do so after the children.
     *  @note   If you overriden pre_*, you should also override post_*.
     */
    virtual auto post_draw() const -> void
    {}
};

/**
 *  @brief   Create a layout.
 *
 *  @tparam  layout_type  Layout type.
 *  @return  Shared pointer to newly created layout.
 */
template<typename layout_type>
[[nodiscard]] inline auto make_layout()
{
    auto shared = std::make_shared<layout_type>();
    return shared;
}

/**
 *  @brief   Create a layout.
 *
 *  @tparam  layout_type  Layout type.
 *  @param   parent       Parent layout reference.
 *  @return  Shared pointer to newly created layout.
 */
template<typename layout_type>
[[nodiscard]] inline auto make_layout(std::weak_ptr<layout> parent)
{
    auto shared = std::make_shared<layout_type>();

    shared->parent = parent;
    if (auto locked = shared->parent.lock())
    {
        locked->children.emplace_back(shared);
    }

    return shared;
}

/**
 *  @brief   Create a layout.
 *
 *  @tparam  layout_type  Layout type.
 *  @param   children     Children layouts.
 *  @return  Shared pointer to newly created layout.
 */
template<typename layout_type>
[[nodiscard]] inline auto make_layout(
    const std::vector<std::shared_ptr<layout>> children
)
{
    auto shared = std::make_shared<layout_type>();

    shared->children = children;
    for (auto &child : shared->children)
    {
        child->parent = shared;
    }

    return shared;
}

/**
 *  @brief   Create a layout.
 *
 *  @tparam  layout_type  Layout type.
 *  @param   parent       Parent layout reference.
 *  @param   children     Children layouts.
 *  @return  Shared pointer to newly created layout.
 */
template<typename layout_type>
[[nodiscard]] inline auto make_layout(
    std::weak_ptr<layout>                      parent,
    const std::vector<std::shared_ptr<layout>> children
)
{
    auto shared = std::make_shared<layout_type>();

    shared->parent = parent;
    if (auto locked = shared->parent.lock())
    {
        locked->children.emplace_back(shared);
    }

    shared->children = children;
    for (auto &child : shared->children)
    {
        child->parent = shared;
    }

    return shared;
}

/**
 *  @brief  Rebound the layout and children (recursively).
 *  @param  layout  Layout to rebound.
 */
constexpr auto rebound(std::shared_ptr<layout> layout) -> void
{
    layout->pre_rebound();

    for (auto &child : layout->children)
    {
        rebound(child);
    }

    layout->post_rebound();
}

/**
 *  @brief  Review the layout and children (recursively).
 *  @param  layout  Layout to review.
 */
constexpr auto review(std::shared_ptr<layout> layout) -> void
{
    layout->pre_review();

    for (auto &child : layout->children)
    {
        review(child);
    }

    layout->post_review();
}

/**
 *  @brief  Update the layout and children (recursively).
 *  @param  layout  Layout to update.
 */
constexpr auto update(std::shared_ptr<layout> layout) -> void
{
    layout->pre_update();

    for (auto &child : layout->children)
    {
        update(child);
    }

    layout->post_update();
}

/**
 *  @brief  Draw the layout and children (recursively).
 *  @param  layout  Layout to draw.
 */
constexpr auto draw(std::shared_ptr<layout> layout) -> void
{
    layout->pre_draw();

    for (auto &child : layout->children)
    {
        draw(child);
    }

    layout->post_draw();
}

/**
 *  @brief  Layout with screen size for bounds.
 */
struct screen_layout : layout {

    /**
     *  @brief  Default constructor.
     */
    screen_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~screen_layout() = default;

    /**
     *  @brief  Layout rebounds before children.
     */
    virtual auto pre_rebound() -> void override
    {
        bounds = { 0.0f, 0.0f, (float)GetScreenWidth(),
            (float)GetScreenHeight() };
    }

    /**
     *  @brief  Layout rebounds after children.
     */
    virtual auto post_rebound() -> void override
    {}
};

/**
 *  @brief  Box bounding rule.
 */
struct box_rule {

    /**
     *  @brief  Offset.
     *
     *  The offset is relative to the center of the parent layout, i.e., the
     *  offset 0 means that the layout is centered perfectly with the center
     *  of its parent.
     *
     *  Variables that can be used in flux:
     *  - `psx`: Parent size X.
     *  - `psy`: Parent size Y.
     *  - `csx`: Current shrink X, evaluated from @c shrink .
     *  - `csy`: Current shrink Y, evaluated from @c shrink .
     */
    flux::flux2 offset;

    /**
     *  @brief  Shrink.
     *
     *  The shrink is inverted from the edges of the parent layout, i.e., the
     *  shrink 0 means that the layout size is the same as the parent's size.
     *
     *  Variables that can be used in flux:
     *  - `psx`: Parent size X.
     *  - `psy`: Parent size Y.
     */
    flux::flux2 shrink;

    /**
     *  @brief  Evaluated rule.
     */
    struct eval {

        /**
         *  @brief  Evaluated offset.
         *  @see    @c box_rule::offset .
         */
        Vector2 offset;

        /**
         *  @brief  Evaluated shrink.
         *  @see    @c box_rule::shrink .
         */
        Vector2 shrink;
    };

    /**
     *  @brief   Evaluate rule.
     *
     *  @param   p_bounds  Parent's bounds.
     *  @return  Evaluated rule.
     */
    [[nodiscard]] inline constexpr auto evaluate(Rectangle p_bounds)
    {
        eval    eval   = {};
        Vector2 p_pos  = { p_bounds.x, p_bounds.y };
        Vector2 p_size = { p_bounds.width, p_bounds.height };

        // Evaluate shrink
        shrink.x.variables["psx"] = p_size.x;
        shrink.x.variables["psy"] = p_size.y;
        shrink.y.variables["psx"] = p_size.x;
        shrink.y.variables["psy"] = p_size.y;
        eval.shrink = shrink;

        // Evaluate offset
        offset.x.variables["psx"] = p_size.x;
        offset.x.variables["psy"] = p_size.y;
        offset.x.variables["csx"] = eval.shrink.x;
        offset.x.variables["csy"] = eval.shrink.y;
        offset.y.variables["psx"] = p_size.x;
        offset.y.variables["psy"] = p_size.y;
        offset.y.variables["csx"] = eval.shrink.x;
        offset.y.variables["csy"] = eval.shrink.y;
        eval.offset = offset;

        return eval;
    }
};

/**
 *  @brief  Bounded layout (layout with custom bounds).
 */
struct box_layout : layout {

    /**
     *  @brief  Box bounding rule.
     */
    box_rule rule;

    /**
     *  @brief  Default constructor.
     */
    box_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~box_layout() = default;

    /**
     *  @brief   Calculate the bounds using @c rule.offset and @c rule.shrink .
     *  @return  Calculated bounds.
     */
    [[nodiscard]] auto calculate_bounds()
    {
        Rectangle p_bounds = { 0.0f, 0.0f, 0.0f, 0.0f };

        if (auto locked = parent.lock())
        {
            p_bounds = locked->bounds;
        }

        Vector2 p_pos  = { p_bounds.x, p_bounds.y };
        Vector2 p_size = { p_bounds.width, p_bounds.height };

        auto eval = rule.evaluate(p_bounds);

        // Total calculations
        Vector2 t_size = p_size - eval.shrink;
        Vector2 t_pos  = eval.offset + p_pos + (p_size - t_size) / 2.0f;

        return Rectangle { t_pos.x, t_pos.y, t_size.x, t_size.y };
    }

    /**
     *  @brief  Layout rebounds before children.
     */
    virtual auto pre_rebound() -> void override
    {
        bounds = calculate_bounds();
    }

    /**
     *  @brief  Layout rebounds after children.
     */
    virtual auto post_rebound() -> void override
    {}
};

/**
 *  @brief  Linear arrangement rule.
 */
struct linear_rule {

    /**
     *  @brief  Minimum shrink.
     *  @see    @c  box_layout::shrink  for what "shrink" means.
     *
     *  Variables that can be used in flux:
     *  - `psx`: Parent size X.
     *  - `psy`: Parent size Y.
     *  - `w`  : Weight, evaluated from @c weight .
     */
    flux::flux min_shrink;

    /**
     *  @brief  Maximum shrink.
     *  @see    @c  box_layout::shrink  for what "shrink" means.
     *
     *  Variables that can be used in flux:
     *  - `psx`: Parent size X.
     *  - `psy`: Parent size Y.
     *  - `w`  : Weight, evaluated from @c weight .
     */
    flux::flux max_shrink;

    /**
     *  @brief  Distribution weight.
     *
     *  This weight system is different from many other GUI framework.  The
     *  weight only represents the proportion of remaining space to be
     *  distributed when you perform parent size minus sum of all the min
     *  sizes of each constraint (p_bounds.wh - total(min_sizes)).
     *
     *  The proportion is applied individually but if the applied size is above
     *  max, it is clamped and the remaining space is to be further distributed.
     *
     *  If there is still gaps leftover, it will be added to beginning and end
     *  of the main axis of the linear layout (i.e., items will be centered
     *  within the layout).
     *
     *  Variables that can be used in flux:
     *  - `psx`: Parent size X.
     *  - `psy`: Parent size Y.
     */
    flux::flux weight;

    /**
     *  @brief  Evaluated rule.
     */
    struct eval {

        /**
         *  @brief  Evaluated minimum shrink.
         *  @see    @c linear_rule::min_shrink .
         */
        float min_shrink;

        /**
         *  @brief  Evaluated maximum shrink.
         *  @see    @c linear_rule::max_shrink .
         */
        float max_shrink;

        /**
         *  @brief  Evaluated distribution weight.
         *  @see    @c linear_rule::weight .
         */
        float weight;
    };

    /**
     *  @brief   Evaluate rule.
     *
     *  @param   p_bounds  Parent's bounds.
     *  @return  Evaluated rule.
     */
    [[nodiscard]] inline constexpr auto evaluate(Rectangle p_bounds)
    {
        eval    eval   = {};
        Vector2 p_pos  = { p_bounds.x, p_bounds.y };
        Vector2 p_size = { p_bounds.width, p_bounds.height };

        // Evaluate weight
        weight.variables["psx"] = p_size.x;
        weight.variables["psy"] = p_size.y;
        eval.weight = weight;

        // Evaluate shrinks
        min_shrink.variables["psx"] = p_size.x;
        min_shrink.variables["psy"] = p_size.y;
        min_shrink.variables["w"]   = eval.weight;
        eval.min_shrink = min_shrink;

        max_shrink.variables["psx"] = p_size.x;
        max_shrink.variables["psy"] = p_size.y;
        max_shrink.variables["w"]   = eval.weight;
        eval.max_shrink = max_shrink;

        return eval;
    }
};

/**
 *  @brief  Linear layout result.
 */
struct linear_layout_result {

    /**
     *  @brief  Linear rule result.
     */
    struct linear_rule_result {

        /**
         *  @brief  Bounds position.
         */
        float pos;

        /**
         *  @brief  Bounds size.
         */
        float size;
    };

    /**
     *  @brief  Linear rules results.
     */
    std::vector<linear_rule_result> rules;

    /**
     *  @brief  Layout new position.
     */
    float new_pos;

    /**
     *  @brief  Layout new size.
     */
    float new_size;
};

/**
 *  @brief   Calculate linear layout result.
 *
 *  @param   layout_pos   Layout's main-axis position.
 *  @param   layout_size  Layout's main-axis size.
 *  @param   l_bounds     Layout's bounds.
 *  @param   rules        Linear layout rules.
 *  @return  Linear layout result.
 */
[[nodiscard]] auto calculate_linear_layout(
    float layout_pos, float layout_size, Rectangle l_bounds,
    std::vector<linear_rule> &rules
) -> linear_layout_result;

/**
 *  @brief  Linear layout's item.
 */
struct linear_item : layout {

    /**
     *  @brief  Linear item's rule.
     */
    linear_rule rule;

    /**
     *  @brief  Default constructor.
     */
    linear_item() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~linear_item() = default;

    /**
     *  @brief  Layout rebounds before children.
     */
    virtual auto pre_rebound() -> void override
    {}

    /**
     *  @brief  Layout rebounds after children.
     */
    virtual auto post_rebound() -> void override
    {}
};

/**
 *  @brief  Column (vertical) layout.
 */
struct column_layout : layout {

    /**
     *  @brief  Default constructor.
     */
    column_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~column_layout() = default;

    /**
     *  @brief  Layout rebounds before children.
     */
    virtual auto pre_rebound() -> void override;

    /**
     *  @brief  Layout rebounds after children.
     */
    virtual auto post_rebound() -> void override
    {}
};

/**
 *  @brief  Row (horizontal) layout.
 */
struct row_layout : layout {

    /**
     *  @brief  Default constructor.
     */
    row_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~row_layout() = default;

    /**
     *  @brief  Layout rebounds before children.
     */
    virtual auto pre_rebound() -> void override;

    /**
     *  @brief  Layout rebounds after children.
     */
    virtual auto post_rebound() -> void override
    {}
};

/**
 *  @brief  Grid arrangement rule.
 *
 *  The item's position and size is specified in Grid Cell Coordinates
 *  (GCC... wait a minute). The integer part describes an indexing value of row
 *  or column, while the fraction part describes offsetting value, relative to
 *  the size of the row or column.
 *
 *  For position, GCC represents and index and an offset for integral part and
 *  fractional part, respectively.
 *
 *  For size, GCC represents the span and extension for integral part and
 *  fractional part, respectively.
 */
struct grid_rule {

    /**
     *  @brief  Position in GCC.
     *
     *  Variables that can be used in flux:
     *  - `psx`: Parent size X.
     *  - `psy`: Parent size Y.
     *  - `csx`: Current size X (shrink coefficient), evaluated from @c size .
     *  - `csy`: Current size Y (shrink coefficient), evaluated from @c size .
     */
    flux::flux2 pos;

    /**
     *  @brief  Size in GCC.
     *
     *  Variables that can be used in flux:
     *  - `psx`: Parent size X.
     *  - `psy`: Parent size Y.
     */
    flux::flux2 size;

    /**
     *  @brief  Evaluated rule.
     */
    struct eval {

        /**
         *  @brief  Evaluated position.
         *  @see    @c grid_rule::min_pos .
         */
        Vector2 pos;

        /**
         *  @brief  Evaluated size.
         *  @see    @c grid_rule::max_size .
         */
        Vector2 size;
    };

    /**
     *  @brief   Evaluate rule.
     *
     *  @param   p_bounds  Parent's bounds.
     *  @return  Evaluated rule.
     */
    [[nodiscard]] inline constexpr auto evaluate(Rectangle p_bounds)
    {
        eval    eval   = {};
        Vector2 p_pos  = { p_bounds.x, p_bounds.y };
        Vector2 p_size = { p_bounds.width, p_bounds.height };

        // Evaluate size
        size.x.variables["psx"] = p_size.x;
        size.x.variables["psy"] = p_size.y;
        size.y.variables["psx"] = p_size.x;
        size.y.variables["psy"] = p_size.y;
        eval.size = size;

        // Evaluate position (offset)
        pos.x.variables["psx"] = p_size.x;
        pos.x.variables["psy"] = p_size.y;
        pos.x.variables["csx"] = eval.size.x;
        pos.x.variables["csy"] = eval.size.y;
        pos.y.variables["psx"] = p_size.x;
        pos.y.variables["psy"] = p_size.y;
        pos.y.variables["csx"] = eval.size.x;
        pos.y.variables["csy"] = eval.size.y;
        eval.pos = pos;

        return eval;
    }
};

/**
 *  @brief  Grid layout item.
 */
struct grid_item : layout {

    /**
     *  @brief  Grid item's rule.
     */
    grid_rule rule;

    /**
     *  @brief  Default constructor.
     */
    grid_item() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~grid_item() = default;

    /**
     *  @brief  Layout rebounds before children.
     */
    virtual auto pre_rebound() -> void override
    {}

    /**
     *  @brief  Layout rebounds after children.
     */
    virtual auto post_rebound() -> void override
    {}
};

/**
 *  @brief  Grid layout.
 */
struct grid_layout : layout {

    /**
     *  @brief  Rows of the grid.
     */
    std::vector<linear_rule> rows;

    /**
     *  @brief  Columns of the grid.
     */
    std::vector<linear_rule> columns;

    /**
     *  @brief  Default constructor.
     */
    grid_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~grid_layout() = default;

    /**
     *  @brief  Layout rebounds before children.
     */
    virtual auto pre_rebound() -> void override;

    /**
     *  @brief  Layout rebounds after children.
     */
    virtual auto post_rebound() -> void override
    {}
};

/**
 *  @brief  Layout which does not review based on parent.
 */
struct unreviewed_layout : layout {

    /**
     *  @brief  Default constructor.
     */
    unreviewed_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~unreviewed_layout() = default;

    /**
     *  @brief  Layout reviews before children.
     */
    virtual auto pre_review() -> void override
    {
        view = { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    /**
     *  @brief  Layout reviews after children.
     */
    virtual auto post_review() -> void override
    {}
};

/**
 *  @brief  Layout with solid color.
 */
struct color_layout : layout {

    /**
     *  @brief  Layout color.
     */
    Color color;

    /**
     *  @brief  Default constructor.
     */
    color_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~color_layout() = default;

    /**
     *  @brief  Layout draws before children.
     */
    virtual auto pre_draw() const -> void override
    {
        DrawRectangleRec(drawable_bounds(), color);
        draw_debug_bounds();
    }

    /**
     *  @brief  Layout draws after children.
     */
    virtual auto post_draw() const -> void override
    {}
};

/**
 *  @brief  Layout with texture.
 */
struct texture_layout : layout {

    /**
     *  @brief  Layout texture.
     */
    Texture texture;

    /**
     *  @brief  Texture draw source.
     */
    Rectangle source;

    /**
     *  @brief  Texture rotation origin.
     */
    Vector2 origin;

    /**
     *  @brief  Texture rotation (in degrees).
     */
    float rotation;

    /**
     *  @brief  Texture draw tint.
     */
    Color tint;

    /**
     *  @brief  Default constructor.
     */
    texture_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~texture_layout() = default;

    /**
     *  @brief  Layout draws before children.
     */
    virtual auto pre_draw() const -> void override
    {
        // FIXME: Undefined (probably squeezed) texture behavior when bounds
        // get reviewed
        DrawTexturePro(texture, source, drawable_bounds(), origin, rotation,
            tint);
        draw_debug_bounds();
    }

    /**
     *  @brief  Layout draws after children.
     */
    virtual auto post_draw() const -> void override
    {}
};

/**
 *  @brief  Layout with text.
 */
struct text_layout : layout {

    /**
     *  @brief  Layout text.
     */
    std::string text;

    /**
     *  @brief  Font to draw text with.
     */
    Font font;

    /**
     *  @brief  Text rotation (in degrees).
     */
    float rotation;

    /**
     *  @brief  Text font size.
     */
    float font_size;

    /**
     *  @brief  Character spacing.
     */
    float spacing;

    /**
     *  @brief  Text color.
     */
    Color tint;

    /**
     *  @brief  Default constructor.
     */
    text_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~text_layout() = default;

    /**
     *  @brief  Layout draws before children.
     */
    virtual auto pre_draw() const -> void override
    {
        Vector2 measurement = MeasureTextEx(font, text.c_str(), font_size,
            spacing);

        Rectangle rec = drawable_bounds();

        DrawTextPro(font, text.c_str(),
            { rec.x + rec.width, rec.y + rec.height },
            { measurement.x, measurement.y },
            rotation, font_size, spacing, tint);
        draw_debug_bounds();
    }

    /**
     *  @brief  Layout draws after children.
     */
    virtual auto post_draw() const -> void override
    {}
};

/**
 *  @brief  Interacttable layout.
 */
struct int_layout : layout {

    /**
     *  @brief  Default constructor.
     */
    int_layout() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~int_layout() = default;

    /**
     *  @brief  Layout updates before children.
     */
    virtual auto pre_update() -> void override;

    /**
     *  @brief  Layout updates after children.
     */
    virtual auto post_update() -> void override;
};

} // namespace minui
