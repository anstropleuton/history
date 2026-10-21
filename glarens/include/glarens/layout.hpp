// Glarens - GUI Framework.
//
// Layout nodes.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#pragma once

#include "glarens/model.hpp"
#include "glarens/node.hpp"
#include <memory>
#include <stdexcept>

enum Align {
    ALIGN_BEGIN,  /// Align to the beginning of the axis
    ALIGN_CENTER, /// Align to the center of the axis
    ALIGN_END,    /// Align to the end of the axis

    // Applicable for multi-child configs only; treated as ALIGN_CENTER for single-child alignment

    ALIGN_SPACE_BETWEEN, /// Align with equal space between each items
    ALIGN_SPACE_EQUALLY, /// Align with equal space between each items and edges
    ALIGN_SPACE_AROUND   /// Align with equal space between each sides of each items
};

enum Direction {
    DIRECTION_UP,   /// y- direction
    DIRECTION_DOWN, /// y+ direction
    DIRECTION_LEFT, /// x- direction
    DIRECTION_RIGHT /// x+ direction
};

// Note: Children without the corresponding Item context are treated as if the context exists with default values

class ClampLayout : public Node {
    Param<Model> min_; /// Minimum model rectangle.
    Param<Model> max_; /// Minimum model rectangle.

    void update_layout_();

  protected:
    ClampLayout() = default;

  public:
    [[nodiscard]] Model get_min() const {
        return min_.get();
    }

    void set_min(const Model &min) {
        min_.set(min);
        update_layout_();
    }

    [[nodiscard]] Model get_max() const {
        return max_.get();
    }

    void set_max(const Model &max) {
        max_.set(max);
        update_layout_();
    }
};

/// Arranges items linearly
class FlexLayout : public Node {
    Param<Direction> dir_         = DIRECTION_RIGHT; /// Direction to flow
    Param<Direction> wrapDir_     = DIRECTION_DOWN;  /// Direction to wrap
    Param<Align>     align_       = ALIGN_BEGIN;     /// Align in the flow
    Param<Align>     wrapAlign_   = ALIGN_BEGIN;     /// Align in the wrap
    Param<bool>      doWrapping_  = true;            /// Enable wrapping
    Param<bool>      altWrapping_ = false;           /// Wrap in alternative direction

    void update_layout_();

  protected:
    FlexLayout() = default;

  public:
    [[nodiscard]] Direction get_dir() const {
        return dir_.get();
    }

    void set_dir(const Direction &value) {
        dir_.set(value);
        update_layout_();
    }

    [[nodiscard]] Direction get_wrap_dir() const {
        return wrapDir_.get();
    }

    void set_wrap_dir(const Direction &value) {
        wrapDir_.set(value);
        update_layout_();
    }

    [[nodiscard]] Align get_align() const {
        return align_.get();
    }

    void set_align(const Align &value) {
        align_.set(value);
        update_layout_();
    }

    [[nodiscard]] Align get_wrap_align() const {
        return wrapAlign_.get();
    }

    void set_wrap_align(const Align &value) {
        wrapAlign_.set(value);
        update_layout_();
    }

    [[nodiscard]] bool get_do_wrapping() const {
        return doWrapping_.get();
    }

    void set_do_wrapping(const bool &value) {
        doWrapping_.set(value);
        update_layout_();
    }

    [[nodiscard]] bool get_alt_wrapping() const {
        return altWrapping_.get();
    }

    void set_alt_wrapping(const bool &value) {
        altWrapping_.set(value);
        update_layout_();
    }

    [[nodiscard]] static std::shared_ptr<FlexLayout> create() {
        return std::shared_ptr<FlexLayout>(new FlexLayout);
    }

    [[nodiscard]] std::shared_ptr<Node> recreate() const override {
        return std::shared_ptr<FlexLayout>(new FlexLayout);
    }

    void sync(const std::shared_ptr<Node> &proto) override {
        auto layout = std::dynamic_pointer_cast<FlexLayout>(proto);
        if (!layout) {
            throw std::runtime_error("Prototype of a different type cannot be used to synchronize parameters");
        }

        Node::sync(proto);

        dir_.set_proto(layout->dir_.get());
        wrapDir_.set_proto(layout->wrapDir_.get());
        align_.set_proto(layout->align_.get());
        wrapAlign_.set_proto(layout->wrapAlign_.get());
        doWrapping_.set_proto(layout->doWrapping_.get());
        altWrapping_.set_proto(layout->altWrapping_.get());
    }
};

class FlexItem : public Context {
    Param<float> weight_     = 1.0f; /// Weighted alignment in flow axis
    Param<float> wrapWeight_ = 1.0f; /// Weighted alignment in wrap axis (averaged for all items in main axis)

    void update_layout_();

  protected:
    FlexItem() = default;

  public:
    [[nodiscard]] float get_weight() const {
        return weight_.get();
    }

    void set_weight(const float &value) {
        weight_.set(value);
        update_layout_();
    }

    [[nodiscard]] float get_wrap_weight() const {
        return wrapWeight_.get();
    }

    void set_wrap_weight(const float &value) {
        wrapWeight_.set(value);
        update_layout_();
    }

    [[nodiscard]] static std::shared_ptr<FlexItem> create() {
        return std::shared_ptr<FlexItem>(new FlexItem);
    }

    std::shared_ptr<Context> clone() override {
        auto clone = create();
        clone->sync(shared_from_this());
        return clone;
    }

    void sync(const std::shared_ptr<Context> &proto) override {
        auto context = std::dynamic_pointer_cast<FlexItem>(proto);
        if (!context) {
            throw std::runtime_error("Prototype of a different type cannot be used to synchronize parameters");
        }

        weight_.set_proto(context->weight_.get());
        wrapWeight_.set_proto(context->wrapWeight_.get());
    }
};

/// Arranges items in a grid
class GridLayout : public Node {
    Param<Direction> dir_       = DIRECTION_RIGHT; /// Direction to flow in slots
    Param<Direction> wrapDir_   = DIRECTION_DOWN;  /// Direction to wrap in slots
    Param<Align>     align_     = ALIGN_BEGIN;     /// Align in the flow
    Param<Align>     wrapAlign_ = ALIGN_BEGIN;     /// Align in the wrap
    Param<int>       slots_     = 1;               /// Number of slots in flow axis
    Param<bool>      masonry_   = false;           /// Whether to arrange next items in main slot in masonry style

    void update_layout_();

  protected:
    GridLayout() = default;

  public:
    [[nodiscard]] Direction get_dir() const {
        return dir_.get();
    }

    void set_dir(const Direction &value) {
        dir_.set(value);
        update_layout_();
    }

    [[nodiscard]] Direction get_wrap_dir() const {
        return wrapDir_.get();
    }

    void set_wrap_dir(const Direction &value) {
        wrapDir_.set(value);
        update_layout_();
    }

    [[nodiscard]] Align get_align() const {
        return align_.get();
    }

    void set_align(const Align &value) {
        align_.set(value);
        update_layout_();
    }

    [[nodiscard]] Align get_wrap_align() const {
        return wrapAlign_.get();
    }

    void set_wrap_align(const Align &value) {
        wrapAlign_.set(value);
        update_layout_();
    }

    [[nodiscard]] int get_slots() const {
        return slots_.get();
    }

    void set_slots(const int &value) {
        slots_.set(value);
        update_layout_();
    }

    [[nodiscard]] bool get_masonry() const {
        return masonry_.get();
    }

    void set_masonry(const bool &value) {
        masonry_.set(value);
        update_layout_();
    }

    [[nodiscard]] static std::shared_ptr<GridLayout> create() {
        return std::shared_ptr<GridLayout>(new GridLayout);
    }

    [[nodiscard]] std::shared_ptr<Node> recreate() const override {
        return std::shared_ptr<GridLayout>(new GridLayout);
    }

    void sync(const std::shared_ptr<Node> &proto) override {
        auto layout = std::dynamic_pointer_cast<GridLayout>(proto);
        if (!layout) {
            throw std::runtime_error("Prototype of a different type cannot be used to synchronize parameters");
        }

        Node::sync(proto);

        dir_.set_proto(layout->dir_.get());
        wrapDir_.set_proto(layout->wrapDir_.get());
        align_.set_proto(layout->align_.get());
        wrapAlign_.set_proto(layout->wrapAlign_.get());
        slots_.set_proto(layout->slots_.get());
        masonry_.set_proto(layout->masonry_.get());
    }
};

class GridItem : public Context {
    Param<int>   xSpan_   = 1;    /// Number of slots to occupy in x-axis
    Param<int>   ySpan_   = 1;    /// Number of slots to occupy in y-axis
    Param<float> xWeight_ = 1.0f; /// Weighted alignment in x-axis (averaged for all items in the slot)
    Param<float> yWeight_ = 1.0f; /// Weighted alignment in y-axis (averaged for all items in the slot)

    void update_layout_();

  protected:
    GridItem() = default;

  public:
    [[nodiscard]] int get_x_span() const {
        return xSpan_.get();
    }

    void set_x_span(const int &value) {
        xSpan_.set(value);
        update_layout_();
    }

    [[nodiscard]] int get_y_span() const {
        return ySpan_.get();
    }

    void set_y_span(const int &value) {
        ySpan_.set(value);
        update_layout_();
    }

    [[nodiscard]] float get_x_weight() const {
        return xWeight_.get();
    }

    void set_x_weight(const float &value) {
        xWeight_.set(value);
        update_layout_();
    }

    [[nodiscard]] float get_y_weight() const {
        return yWeight_.get();
    }

    void set_y_weight(const float &value) {
        yWeight_.set(value);
        update_layout_();
    }

    [[nodiscard]] static std::shared_ptr<GridItem> create() {
        return std::shared_ptr<GridItem>(new GridItem);
    }

    std::shared_ptr<Context> clone() override {
        auto clone = create();
        clone->sync(shared_from_this());
        return clone;
    }

    void sync(const std::shared_ptr<Context> &proto) override {
        auto context = std::dynamic_pointer_cast<GridItem>(proto);
        if (!context) {
            throw std::runtime_error("Prototype of a different type cannot be used to synchronize parameters");
        }

        xSpan_.set_proto(context->xSpan_.get());
        ySpan_.set_proto(context->ySpan_.get());
        xWeight_.set_proto(context->xWeight_.get());
        yWeight_.set_proto(context->yWeight_.get());
    }
};

/// Arranges items in a fixed grid
class MatrixLayout : public Node {
    Param<int>       xSlots_    = 1;               /// Number of slots in x-axis
    Param<int>       ySlots_    = 1;               /// Number of slots in y-axis
    Param<Direction> dir_       = DIRECTION_RIGHT; /// Direction to flow in slots
    Param<Direction> wrapDir_   = DIRECTION_DOWN;  /// Direction to wrap in slots
    Param<Align>     align_     = ALIGN_BEGIN;     /// Align in the flow
    Param<Align>     wrapAlign_ = ALIGN_BEGIN;     /// Align in the wrap

    void update_layout_();

  protected:
    MatrixLayout() = default;

  public:
    [[nodiscard]] int get_x_slots() const {
        return xSlots_.get();
    }

    void set_x_slots(const int &value) {
        xSlots_.set(value);
        update_layout_();
    }

    [[nodiscard]] int get_y_slots() const {
        return ySlots_.get();
    }

    void set_y_slots(const int &value) {
        ySlots_.set(value);
        update_layout_();
    }

    [[nodiscard]] Direction get_dir() const {
        return dir_.get();
    }

    void set_dir(const Direction &value) {
        dir_.set(value);
        update_layout_();
    }

    [[nodiscard]] Direction get_wrap_dir() const {
        return wrapDir_.get();
    }

    void set_wrap_dir(const Direction &value) {
        wrapDir_.set(value);
        update_layout_();
    }

    [[nodiscard]] Align get_align() const {
        return align_.get();
    }

    void set_align(const Align &value) {
        align_.set(value);
        update_layout_();
    }

    [[nodiscard]] Align get_wrap_align() const {
        return wrapAlign_.get();
    }

    void set_wrap_align(const Align &value) {
        wrapAlign_.set(value);
        update_layout_();
    }

    [[nodiscard]] static std::shared_ptr<MatrixLayout> create() {
        return std::shared_ptr<MatrixLayout>(new MatrixLayout);
    }

    [[nodiscard]] std::shared_ptr<Node> recreate() const override {
        return std::shared_ptr<MatrixLayout>(new MatrixLayout);
    }

    void sync(const std::shared_ptr<Node> &proto) override {
        auto layout = std::dynamic_pointer_cast<MatrixLayout>(proto);
        if (!layout) {
            throw std::runtime_error("Prototype of a different type cannot be used to synchronize parameters");
        }

        Node::sync(proto);

        xSlots_.set_proto(layout->xSlots_.get());
        ySlots_.set_proto(layout->ySlots_.get());
        dir_.set_proto(layout->dir_.get());
        wrapDir_.set_proto(layout->wrapDir_.get());
        align_.set_proto(layout->align_.get());
        wrapAlign_.set_proto(layout->wrapAlign_.get());
    }
};

class MatrixItem : public Context {
    Param<int>   xSpan_   = 1;    /// Number of slots to occupy in x-axis
    Param<int>   ySpan_   = 1;    /// Number of slots to occupy in y-axis
    Param<float> xWeight_ = 1.0f; /// Weighted alignment in x-axis (averaged for all items in the slot)
    Param<float> yWeight_ = 1.0f; /// Weighted alignment in y-axis (averaged for all items in the slot)

    void update_layout_();

  protected:
    MatrixItem() = default;

  public:
    [[nodiscard]] int get_x_span() const {
        return xSpan_.get();
    }

    void set_x_span(const int &value) {
        xSpan_.set(value);
        update_layout_();
    }

    [[nodiscard]] int get_y_span() const {
        return ySpan_.get();
    }

    void set_y_span(const int &value) {
        ySpan_.set(value);
        update_layout_();
    }

    [[nodiscard]] float get_x_weight() const {
        return xWeight_.get();
    }

    void set_x_weight(const float &value) {
        xWeight_.set(value);
        update_layout_();
    }

    [[nodiscard]] float get_y_weight() const {
        return yWeight_.get();
    }

    void set_y_weight(const float &value) {
        yWeight_.set(value);
        update_layout_();
    }

    [[nodiscard]] static std::shared_ptr<MatrixItem> create() {
        return std::shared_ptr<MatrixItem>(new MatrixItem);
    }

    std::shared_ptr<Context> clone() override {
        auto clone = create();
        clone->sync(shared_from_this());
        return clone;
    }

    void sync(const std::shared_ptr<Context> &proto) override {
        auto context = std::dynamic_pointer_cast<MatrixItem>(proto);
        if (!context) {
            throw std::runtime_error("Prototype of a different type cannot be used to synchronize parameters");
        }

        xSpan_.set_proto(context->xSpan_.get());
        ySpan_.set_proto(context->ySpan_.get());
        xWeight_.set_proto(context->xWeight_.get());
        yWeight_.set_proto(context->yWeight_.get());
    }
};

/// Arranges items by splitting view recursively based on aspect ratio
/// Note: does not traverse the children. It only arranges the immediate children in abstract split space
class SplitLayout : public Node {
    using Vec = std::vector<float>;

    Param<Vec>       splitWeights_ = Vec{1.0f, 1.0f}; /// Number of splits and weight of each split
    Param<Direction> splitDir_     = DIRECTION_RIGHT; /// Direction to split primarily
    Param<Direction> wrapDir_      = DIRECTION_DOWN;  /// Direction to split alternatively
    Param<int>       minDepth_     = 0;               /// Minimum split depth initially

    void update_layout_();

  protected:
    SplitLayout() = default;

  public:
    [[nodiscard]] Vec get_split_weights() const {
        return splitWeights_.get();
    }

    void set_split_weights(const Vec &value) {
        splitWeights_.set(value);
        update_layout_();
    }

    [[nodiscard]] Direction get_splitDir_() const {
        return splitDir_.get();
    }

    void set_splitDir_(const Direction &value) {
        splitDir_.set(value);
        update_layout_();
    }

    [[nodiscard]] Direction get_wrap_dir() const {
        return wrapDir_.get();
    }

    void set_wrap_dir(const Direction &value) {
        wrapDir_.set(value);
        update_layout_();
    }

    [[nodiscard]] int get_minDepth_() const {
        return minDepth_.get();
    }

    void set_minDepth_(const int &value) {
        minDepth_.set(value);
        update_layout_();
    }

    [[nodiscard]] static std::shared_ptr<SplitLayout> create() {
        return std::shared_ptr<SplitLayout>(new SplitLayout);
    }

    [[nodiscard]] std::shared_ptr<Node> recreate() const override {
        return std::shared_ptr<SplitLayout>(new SplitLayout);
    }

    void sync(const std::shared_ptr<Node> &proto) override {
        auto layout = std::dynamic_pointer_cast<SplitLayout>(proto);
        if (!layout) {
            throw std::runtime_error("Prototype of a different type cannot be used to synchronize parameters");
        }

        Node::sync(proto);

        splitWeights_.set_proto(layout->splitWeights_.get());
        splitDir_.set_proto(layout->splitDir_.get());
        wrapDir_.set_proto(layout->wrapDir_.get());
        minDepth_.set_proto(layout->minDepth_.get());
    }
};

class SplitItem : public Context {
    void update_layout_();

  protected:
    SplitItem() = default;

  public:
    [[nodiscard]] static std::shared_ptr<SplitItem> create() {
        return std::shared_ptr<SplitItem>(new SplitItem);
    }

    std::shared_ptr<Context> clone() override {
        auto clone = create();
        clone->sync(shared_from_this());
        return clone;
    }

    void sync(const std::shared_ptr<Context> &proto) override {
        auto context = std::dynamic_pointer_cast<SplitItem>(proto);
        if (!context) {
            throw std::runtime_error("Prototype of a different type cannot be used to synchronize parameters");
        }
    }
};
