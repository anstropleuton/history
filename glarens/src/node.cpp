// Glarens - GUI Framework.
//
// GUI Node Graph implementation.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#include "glarens/node.hpp"
#include "internal/app-data.hpp"
#include "internal/utils.hpp"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstddef>
#include <format>
#include <functional>
#include <glm/glm.hpp>
#include <unordered_map>

static int idCounter = 0;

static std::unordered_map<std::size_t, float> debug_hues;

void Node::refresh_model() const {
    Rect      parentModelRect      = Rect::screen_rect();
    glm::mat3 parentWorldTransform = glm::mat3(1.0f);

    if (auto parent = parent_.lock()) {
        parentModelRect      = parent->modelRect_;
        parentWorldTransform = parent->worldTransform_;
    }

    modelRect_      = model_rect(model_.get(), parentModelRect);
    Quad modelQuad_ = model_quad(modelRect_);
    worldTransform_ = parentWorldTransform * transform_.get();
    transformQuad_  = worldTransform_ * modelQuad_;
}

void Node::debug() const {
    std::size_t this_size = std::size_t(this);
    float       hue       = 0.0f;
    if (!debug_hues.contains(this_size)) {
        hue = (std::hash<std::size_t>()(this_size) % 36) / 36.0f;
        debug_hues.insert_or_assign(this_size, hue);
    } else {
        hue = debug_hues.at(this_size);
    }

    auto rgb = hsl2rgb(hue, 1.0f, 0.5f);
    draw_quad_lines(appData.renderer, transformQuad_, SDL_Color{rgb[0], rgb[1], rgb[2], 255});
    draw_quad_fill(appData.renderer, transformQuad_, SDL_Color{rgb[0], rgb[1], rgb[2], 63});

    for (const auto &child : children_) {
        child->debug();
    }
}

void set_root(std::shared_ptr<Node> root) {
    appData.root = root;
    root->refresh_model();
}

[[nodiscard]] std::shared_ptr<Node> get_root() {
    return appData.root;
}

void iterate_root() {
    appData.root->traverse_down([](auto item) { item->refresh_model(); });
    appData.root->traverse_down([](auto item) { item->model(); });
    appData.root->traverse_down([](auto item) { item->update(); });
    appData.root->traverse_down([](auto item) { item->render(); });
    appData.root->traverse_down([](auto item) { item->debug(); });
}
