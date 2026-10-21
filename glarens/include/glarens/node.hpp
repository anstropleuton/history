// Glarens - GUI Framework.
//
// GUI Node Graph.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#pragma once

#include "model.hpp"
#include <algorithm>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <optional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

template <typename T>
class Param {
    T                proto = T(); /// Prototype value
    std::optional<T> local;       /// Local override

  public:
    Param()                              = default;
    Param(const Param &param)            = default;
    Param(Param &&param)                 = default;
    Param &operator=(const Param &param) = default;
    Param &operator=(Param &&param)      = default;

    Param(const T &value) { set(value); }
    Param(T &&value) { set(std::move(value)); }

    Param &operator=(const T &value) {
        set(value);
        return *this;
    }

    Param &operator=(T &&value) {
        set(std::move(value));
        return *this;
    }

    [[nodiscard]] const T &get() const {
        if (local.has_value()) return *local;
        return proto;
    }

    void set(const T &value) { local = value; }
    void set(T &&value) { local = std::move(value); }

    void set_proto(const T &value) { proto = value; }
    void set_proto(T &&value) { proto = std::move(value); }

    void clear() noexcept { local.reset(); }

    [[nodiscard]] bool has_override() const noexcept {
        return local.has_value();
    }
};

class Context : public std::enable_shared_from_this<Context> {
  protected:
    Context() = default;

  public:
    virtual ~Context() = default;

    virtual std::shared_ptr<Context> clone()                                     = 0;
    virtual void                     sync(const std::shared_ptr<Context> &proto) = 0;
};

class Node : public std::enable_shared_from_this<Node> {
    Param<bool> enableModel_          = true;  /// Enable model updating the node. Note: model updating is enabled if parent's model updating and parent's children model updating is enabled
    Param<bool> enableModelChildren_  = true;  /// Enable model updating the node's children
    Param<bool> enableUpdate_         = true;  /// Enable updating the node
    Param<bool> enableUpdateChildren_ = true;  /// Enable updating the node's children
    Param<bool> enableRender_         = true;  /// Enable rendering the node
    Param<bool> enableRenderChildren_ = true;  /// Enable rendering the node's children
    Param<bool> enableDebug_          = false; /// Enable debug rendering the node. Note: debug rendering is enabled if parent's debug rendering and parent's children debug rendering is enabled
    Param<bool> enableDebugChildren_  = true;  /// Enable debug rendering the node's children

    Param<Model>     model_;                       /// Node's bounding box model
    Param<glm::mat3> transform_ = glm::mat3(1.0f); /// Transformation for node's bounding box

    mutable Rect      modelRect_      = Rect::screen_rect();
    mutable glm::mat3 worldTransform_ = glm::mat3(1.0f);
    mutable Quad      transformQuad_;

    std::weak_ptr<Node>                parent_;
    std::vector<std::shared_ptr<Node>> children_;

    std::weak_ptr<Node>              proto_;
    std::vector<std::weak_ptr<Node>> clones_;

    std::unordered_map<std::type_index, std::shared_ptr<Context>> contexts_;

  protected:
    Node() = default;

  public:
    virtual ~Node() = default;

    [[nodiscard]] bool get_enable_model() const {
        return enableModel_.get();
    }

    void set_enable_model(bool value) {
        enableModel_.set(value);
    }

    [[nodiscard]] bool get_enable_model_children() const {
        return enableModelChildren_.get();
    }

    void set_enable_model_children(bool value) {
        enableModelChildren_.set(value);
    }

    [[nodiscard]] bool get_enable_update() const {
        return enableUpdate_.get();
    }

    void set_enable_update(bool value) {
        enableUpdate_.set(value);
    }

    [[nodiscard]] bool get_enable_update_children() const {
        return enableUpdateChildren_.get();
    }

    void set_enable_update_children(bool value) {
        enableUpdateChildren_.set(value);
    }

    [[nodiscard]] bool get_enable_render() const {
        return enableRender_.get();
    }

    void set_enable_render(bool value) {
        enableRender_.set(value);
    }

    [[nodiscard]] bool get_enable_render_children() const {
        return enableRenderChildren_.get();
    }

    void set_enable_render_children(bool value) {
        enableRenderChildren_.set(value);
    }

    [[nodiscard]] bool get_enable_debug() const {
        return enableDebug_.get();
    }

    void set_enable_debug(bool value) {
        enableDebug_.set(value);
    }

    [[nodiscard]] bool get_enable_debug_children() const {
        return enableDebugChildren_.get();
    }

    void set_enable_debug_children(bool value) {
        enableDebugChildren_.set(value);
    }

    [[nodiscard]] Model get_model() const {
        return model_.get();
    }

    /// Changes causes quads refresh
    void set_model(const Model &value) {
        model_.set(value);
        refresh_model();
    }

    [[nodiscard]] glm::mat3 get_transform() const {
        return transform_.get();
    }

    /// Changes causes quads refresh
    void set_transform(const glm::mat3 &value) {
        transform_.set(value);
        refresh_model();
    }

    /// Note: propagates updates down to children
    void refresh_model() const;

    /// Derived must provide their own independent factory
    [[nodiscard]] static std::shared_ptr<Node> create() {
        return std::shared_ptr<Node>(new Node());
    }

    /// Derived must provide their own polymorphic factory
    [[nodiscard]] virtual std::shared_ptr<Node> recreate() const {
        return std::shared_ptr<Node>(new Node());
    }

    /// Derived must provide their own sync function
    virtual void sync(const std::shared_ptr<Node> &proto) {
        enableModel_.set_proto(proto->enableModel_.get());
        enableModelChildren_.set_proto(proto->enableModelChildren_.get());
        enableUpdate_.set_proto(proto->enableUpdate_.get());
        enableUpdateChildren_.set_proto(proto->enableUpdateChildren_.get());
        enableRender_.set_proto(proto->enableRender_.get());
        enableRenderChildren_.set_proto(proto->enableRenderChildren_.get());

        model_.set_proto(proto->model_.get());
        transform_.set_proto(proto->transform_.get());
    }

    [[nodiscard]] std::shared_ptr<Node> clone() {
        auto clone = recreate();

        clone->sync(shared_from_this());

        clones_.push_back(clone);
        clone->proto_ = shared_from_this();

        for (const auto &[type, context] : contexts_) {
            clone->contexts_[type] = context->clone();
        }

        for (const auto &child : children_) {
            clone->insert_child(child->clone());
        }

        return clone;
    }

    [[nodiscard]] std::shared_ptr<Node> clone_single() {
        auto clone = recreate();

        clone->sync(shared_from_this());

        clones_.push_back(clone);
        clone->proto_ = shared_from_this();

        for (const auto &[type, context] : contexts_) {
            clone->contexts_[type] = context->clone();
        }

        return clone;
    }

    void sync_clones() {
        clones_.erase(std::remove_if(clones_.begin(), clones_.end(), [](const std::weak_ptr<Node> &clone) { return clone.expired(); }), clones_.end());
        for (const auto &clone_ : clones_) {
            if (auto clone = clone_.lock()) {
                clone->sync(shared_from_this());

                for (const auto &[type, context] : contexts_) {
                    if (auto it = clone->contexts_.find(type); it != clone->contexts_.end()) {
                        it->second->sync(context);
                    }
                }
            }
        }

        for (const auto &child : children_) {
            child->sync_clones();
        }
    }

    // Note: node tree graph management does not enforce anything

    void insert_child(std::shared_ptr<Node> child) {
        child->parent_ = shared_from_this();
        child->refresh_model();
        children_.push_back(std::move(child));
    }

    template <typename T = Node, typename... Args>
        requires std::is_base_of_v<Node, T> || std::is_same_v<Node, T>
    [[nodiscard]] std::shared_ptr<T> create_child(Args &&...args) {
        auto node = T::create(std::forward<Args>(args)...);
        insert_child(node);
        return node;
    }

    bool remove_child(const std::shared_ptr<Node> &child) {
        auto it = std::find_if(children_.begin(), children_.end(), [&child](const std::shared_ptr<Node> &element) { return child.get() == element.get(); });
        if (it == children_.end()) {
            return false;
        }
        (*it)->parent_.reset();
        (*it)->refresh_model();
        children_.erase(it);
        return true;
    }

    [[nodiscard]] bool has_child(const std::shared_ptr<Node> &child) const noexcept {
        return std::find_if(children_.begin(), children_.end(), [&child](const std::shared_ptr<Node> &element) { return child.get() == element.get(); }) != children_.end();
    }

    [[nodiscard]] std::shared_ptr<Node> get_child(const std::shared_ptr<Node> &child) const {
        auto it = std::find_if(children_.begin(), children_.end(), [&child](const std::shared_ptr<Node> &element) { return child.get() == element.get(); });
        if (it == children_.end()) {
            return nullptr;
        }
        return *it;
    }

    template <typename T>
        requires std::is_base_of_v<Context, T>
    void set_context(std::shared_ptr<T> context) {
        std::type_index type = typeid(T);
        contexts_[type]      = context;
    }

    template <typename T>
        requires std::is_base_of_v<Context, T>
    void reset_context() {
        std::type_index type = typeid(T);
        contexts_.erase(type);
    }

    template <typename T>
        requires std::is_base_of_v<Context, T>
    [[nodiscard]] bool has_context() const noexcept {
        std::type_index type = typeid(T);
        return contexts_.contains(type);
    }

    template <typename T>
        requires std::is_base_of_v<Context, T>
    [[nodiscard]] std::shared_ptr<T> get_context() const {
        if (!has_context<T>()) {
            return nullptr;
        }

        std::type_index type = typeid(T);
        return std::static_pointer_cast<T>(contexts_.at(type));
    }

    /// Checks for context in parent recursively (excluding itself)
    template <typename T>
        requires std::is_base_of_v<Context, T>
    [[nodiscard]] bool has_context_in_ancestor() const noexcept {
        if (auto parent = parent_.lock()) {
            if (parent->has_context<T>()) {
                return true;
            }

            return parent->has_context_in_ancestor<T>();
        }

        return false;
    }

    /// Checks for context in children recursively (excluding itself)
    template <typename T>
        requires std::is_base_of_v<Context, T>
    [[nodiscard]] bool has_context_in_descendant() const noexcept {
        for (const auto &child : children_) {
            if (child->has_context<T>()) {
                return true;
            }

            if (child->has_context_in_descendant<T>()) {
                return true;
            }
        }

        return false;
    }

    /// Obtains context in parent recursively (excluding itself)
    template <typename T>
        requires std::is_base_of_v<Context, T>
    [[nodiscard]] std::shared_ptr<T> get_context_in_ancestor() const {
        if (auto parent = parent_.lock()) {
            if (auto context = parent->get_context<T>()) {
                return context;
            }

            return parent->get_context_in_ancestor<T>();
        }

        return nullptr;
    }

    /// Obtains context in children recursively (excluding itself)
    template <typename T>
        requires std::is_base_of_v<Context, T>
    [[nodiscard]] std::shared_ptr<T> get_context_in_descendant() const {
        for (const auto &child : children_) {
            if (auto context = child->get_context<T>()) {
                return context;
            }

            if (auto context = child->get_context_in_descendant<T>()) {
                return context;
            }
        }

        return nullptr;
    }

    template <typename Pred>
    void traverse_down(Pred pred) {
        pred(shared_from_this());

        for (auto &child : children_) {
            child->traverse_down(pred);
        }
    }

    template <typename Pred>
    void traverse_up(Pred pred) {
        pred(shared_from_this());

        if (auto parent = parent_.lock()) {
            parent->traverse_up(pred);
        }
    }

    /// Override this for modeling
    /// Called after quads refresh
    virtual void model() {}

    /// Override this for updating
    virtual void update() {}

    /// Override this for rendering
    virtual void render() const {}

    /// Override this for rendering custom debug info
    virtual void debug() const;
};

/// Set the global main node of Glarens
void set_root(std::shared_ptr<Node> root);

/// Get the global main node of Glarens
[[nodiscard]] std::shared_ptr<Node> get_root();

/// Model, update and render (and debug render if enabled) the GUI node graph starting from root
void iterate_root();
