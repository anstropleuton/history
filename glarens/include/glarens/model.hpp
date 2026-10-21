// Glarens - GUI Framework.
//
// Node modeling and transformation.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#pragma once

#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/vector_float2.hpp>

enum ReferenceMode {
    REF_MODE_RELATIVE, /// Uses parent quad for modeling reference.
    REF_MODE_ABSOLUTE  /// Uses screen quad for modeling reference.
};

struct Model {
    glm::vec2 pos;      /// Offset from reference position.
    glm::vec2 anchor;   /// Additional offset from reference size.
    glm::vec2 floating; /// Additional offset from current size.
    glm::vec2 size;     /// Absolute extent.
    glm::vec2 scale;    /// Additional extent from reference size.

    ReferenceMode positioningRefMode = REF_MODE_RELATIVE;
    ReferenceMode sizingRefMode      = REF_MODE_RELATIVE;
};

struct Rect {
    glm::vec2 center;
    glm::vec2 extent;

    [[nodiscard]] static Rect screen_rect();
};

struct Quad {
    glm::vec2 tl;
    glm::vec2 tr;
    glm::vec2 bl;
    glm::vec2 br;
};

// Transformation helpers

[[nodiscard]] inline glm::mat3 rotate(float a) noexcept {
    float     c = cos(a);
    float     s = sin(a);
    glm::vec2 v;
    v.x;
    return glm::mat3(
        c, -s, 0,
        s, c, 0,
        0, 0, 1
    );
}

[[nodiscard]] inline glm::mat3 rotate(glm::vec2 v) noexcept {
    float a = atan2(v.y, v.x);
    return rotate(a);
}

[[nodiscard]] inline glm::mat3 scale(glm::vec2 v) noexcept {
    return glm::mat3(
        v.x, 0, 0,
        0, v.y, 0,
        0, 0, 1
    );
}

[[nodiscard]] inline glm::mat3 shear(glm::vec2 v) noexcept {
    return glm::mat3(
        1, v.x, 0,
        v.y, 1, 0,
        0, 0, 1
    );
}

[[nodiscard]] inline glm::mat3 project(glm::vec2 v) noexcept {
    return glm::mat3(
        v.x * v.x, v.x * v.y, 0,
        v.y * v.x, v.y * v.y, 0,
        0, 0, 1
    );
}

[[nodiscard]] inline glm::mat3 reflect(glm::vec2 v) noexcept {
    glm::mat3 p = project(v);
    return glm::mat3(1.0f) - 2.0f * p;
}

[[nodiscard]] inline glm::mat3 translate(glm::vec2 v) noexcept {
    return glm::mat3(
        1, 0, v.x,
        0, 1, v.y,
        0, 0, 1
    );
}

[[nodiscard]] inline glm::mat3 perspective(glm::vec2 v) noexcept {
    return glm::mat3(
        1, 0, 0,
        0, 1, 0,
        v.x, v.y, 1
    );
}

[[nodiscard]] inline glm::vec2 center(glm::mat3 m) noexcept {
    return glm::vec2(m * glm::vec3(0.0f, 0.0f, 1.0f));
}

// Provide screen quad in case of no parent

[[nodiscard]] inline Rect model_rect(Model model, Rect parent) noexcept {
    Rect screenQuad = Rect::screen_rect();

    glm::vec2 refPos  = model.positioningRefMode == REF_MODE_ABSOLUTE ? screenQuad.center : parent.center;
    glm::vec2 refSize = model.sizingRefMode == REF_MODE_ABSOLUTE ? screenQuad.extent : parent.extent;

    glm::vec2 finalSize = model.size + model.scale * refSize;
    glm::vec2 finalPos  = model.pos + refPos + model.anchor * refSize + model.floating * finalSize;

    return Rect{
        .center = finalPos,
        .extent = finalSize,
    };
}

[[nodiscard]] inline Quad model_quad(Rect rect) noexcept {
    return Quad{
        .tl = (rect.center + rect.extent * glm::vec2(-1.0f, -1.0f)),
        .tr = (rect.center + rect.extent * glm::vec2(+1.0f, -1.0f)),
        .bl = (rect.center + rect.extent * glm::vec2(-1.0f, +1.0f)),
        .br = (rect.center + rect.extent * glm::vec2(+1.0f, +1.0f)),
    };
}

[[nodiscard]] inline Quad operator*(glm::mat3 m, Quad quad) noexcept {
    glm::vec3 tl = m * glm::vec3(quad.tl, 1.0f);
    glm::vec3 tr = m * glm::vec3(quad.tr, 1.0f);
    glm::vec3 bl = m * glm::vec3(quad.bl, 1.0f);
    glm::vec3 br = m * glm::vec3(quad.br, 1.0f);

    return Quad{
        .tl = glm::vec2(tl) / tl.z,
        .tr = glm::vec2(tr) / tl.z,
        .bl = glm::vec2(bl) / tl.z,
        .br = glm::vec2(br) / tl.z
    };
}
