/// @file
///
/// @author    Anstro Pleuton <https://github.com/anstropleuton>
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Hydope - Hypnotizing Double Pendulum.
///
/// This source file contains implementaion for pendulum.
///
/// This project is licensed under the terms of MIT license.

#include <raylib.h>

#include <cmath>
#include <cstddef>

#include "pendulum.hpp"
#include "utilities.hpp"

void WeldedPendulum::Update()
{
    const std::size_t n = pendulums.size();
    if (n == 0) return;

    constexpr double g  = 0.981;
    constexpr double dt = 0.1666667;

    if (n == 1)
    {
        Pendulum &p = pendulums[0];
        p.a_acc     = -g / p.length * sin(p.angle);
        p.a_vel += p.a_acc * dt;
        p.angle += p.a_vel * dt;

        p.position = {
            p.length * sin(p.angle),
            p.length * cos(p.angle)
        };

        return;
    }

    // Precompute everything vectorizable or reused
    for (std::size_t i = 0; i < n - 1; i++)
    {
        Pendulum &p1 = pendulums[i];
        Pendulum &p2 = pendulums[i + 1];

        double a1 = p1.angle, a2 = p2.angle;
        double m1 = p1.mass, m2 = p2.mass;
        double l1 = p1.length, l2 = p2.length;

        double sa1 = sin(a1), sa2 = sin(a2);
        double ca1 = cos(a1), ca2 = cos(a2);
        double s12 = sin(a1 - a2), c12 = cos(a1 - a2);
        double s2a1a2 = sin(2.0 * (a1 - a2));
        double c2a1a2 = cos(2.0 * (a1 - a2));

        double n1 = -g * (2.0 * m1 + m2) * sa1;
        double n2 = -m2 * g * sin(a1 - 2.0 * a2);
        double n3 = -2.0 * s12 * m2;
        double n4 = p2.a_vel * p2.a_vel * l2 + p1.a_vel * p1.a_vel * l1 * c12;
        double d  = l1 * (2.0 * m1 + m2 - m2 * cos(2.0 * a1 - 2.0 * a2));
        p1.a_acc  = (n1 + n2 + n3 * n4) / d;

        n1       = 2.0 * s12;
        n2       = p1.a_vel * p1.a_vel * l1 * (m1 + m2);
        n3       = g * (m1 + m2) * ca1;
        n4       = p2.a_vel * p2.a_vel * l2 * m2 * c12;
        d        = l2 * (2.0 * m1 + m2 - m2 * cos(2.0 * a1 - 2.0 * a2));
        p2.a_acc = (n1 * (n2 + n3 + n4)) / d;
    }

    // Vector-friendly loop: remove branch
    Vector2Double prev_pos = { 0.0, 0.0 };

    for (std::size_t i = 0; i < n; i++)
    {
        Pendulum &p = pendulums[i];

        p.a_vel += p.a_acc * dt;
        p.angle += p.a_vel * dt;

        Vector2Double offset = {
            p.length * sin(p.angle),
            p.length * cos(p.angle)
        };

        p.position = {
            prev_pos.x + offset.x,
            prev_pos.y + offset.y
        };

        prev_pos = p.position;
    }

    trajectories[traj_head] = pendulums[n - 1].position;
    traj_head               = (traj_head + 1) % trajectories.size();
}

void WeldedPendulum::Draw() const
{
    const std::size_t len = trajectories.size();

    for (std::size_t i = 0; i < len - 1; i++)
    {
        const auto &current = trajectories[(traj_head + i) % len];
        const auto &next    = trajectories[(traj_head + i + 1) % len];

        if (current == Vector2Double(0.0f, 0.0f)) continue;
        if (next == Vector2Double(0.0f, 0.0f)) continue;

        // Faster approximation than pow(x, 10)
        float t     = (i + 1.0f) / len;
        float alpha = t * t; // linear or quadratic fade
        Color faded = color;
        faded.a     = static_cast<unsigned char>(faded.a * alpha);

        DrawLineV(current, next, faded);
    }
}
