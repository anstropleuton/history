#include "node.hpp"

#include "raymath.h"

auto bppui::model::compute(const Rectangle parent) const -> Rectangle {
    const Vector2 parent_pos{rec_pos(parent)};
    const Vector2 parent_size{rec_size(parent)};
    const Vector2 final_size{extent + scale * parent_size};
    const Vector2 final_pos{parent_pos * (relative ? 1.0F : 0.0F) + offset + anchor * parent_size - pivot * final_size};
    return pos_size_rec(final_pos, final_size);
}

auto bppui::node::update_depth_() -> void {
    depth_ = parent_ == nullptr ? 0 : parent_->depth_;
    for (const auto &child : children_) {
        child->update_depth_();
    }
}

auto bppui::node::is_ancestor_of(const node *const descendant) const -> bool {
    for (const node *current{descendant}; current != nullptr; current = current->parent_) {
        if (current == this) return true;
    }

    return false;
}

auto bppui::node::insert(std::unique_ptr<node> child, const std::size_t index) -> node * {
    if (child == nullptr) {
        throw std::invalid_argument("child is nullptr");
    }

    if (child.get() == this) {
        throw std::invalid_argument("cannot parent node to itself");
    }

    if (child->is_ancestor_of(this)) {
        throw std::invalid_argument("cycle detected");
    }

    if (!child->is_root()) {
        throw std::invalid_argument("child is not detached");
    }

    if (index > children_.size()) {
        throw std::out_of_range("index out of range");
    }

    const auto  iter{children_.insert(children_.begin() + static_cast<std::ptrdiff_t>(index), std::move(child))};
    node *const current{iter->get()};
    node *const prev{index > 0 ? children_[index - 1].get() : nullptr};
    node *const next{index + 1 < children_.size() ? children_[index + 1].get() : nullptr};

    current->parent_        = this;
    current->prev_sibling_  = prev;
    current->next_sibling_  = next;
    current->sibling_index_ = index;

    if (prev != nullptr) {
        prev->next_sibling_ = current;
    }
    if (next != nullptr) {
        next->prev_sibling_ = current;
    }

    for (std::size_t remain{index + 1}; remain < children_.size(); ++remain) {
        ++children_[remain]->sibling_index_;
    }

    dirty_ = true;

    current->update_depth_();

    return current;
}

auto bppui::node::append(std::unique_ptr<node> child) -> node * {
    return insert(std::move(child), children_.size());
}

auto bppui::node::detach() -> std::unique_ptr<node> {
    if (is_root()) {
        throw std::runtime_error("root node is not detached");
    }

    const auto iter{parent_->children_.begin() + static_cast<std::ptrdiff_t>(sibling_index_)};
    auto       detached{std::move(*iter)};

    parent_->children_.erase(iter);

    for (std::size_t remain{sibling_index_}; remain < parent_->children_.size(); ++remain) {
        --parent_->children_[remain]->sibling_index_;
    }

    parent_->dirty_ = true;
    dirty_          = true;

    if (prev_sibling_ != nullptr) {
        prev_sibling_->next_sibling_ = next_sibling_;
    }

    if (next_sibling_ != nullptr) {
        next_sibling_->prev_sibling_ = prev_sibling_;
    }

    parent_        = nullptr;
    prev_sibling_  = nullptr;
    next_sibling_  = nullptr;
    sibling_index_ = 0;

    update_depth_();

    return detached;
}
