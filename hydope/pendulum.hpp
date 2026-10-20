/// @file
///
/// @author    Anstro Pleuton <https://github.com/anstropleuton>
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Hydope - Hypnotizing Double Pendulum.
///
/// This header file contains pendulum declaration for Hydope.
///
/// This project is licensed under the terms of MIT license.

#pragma once

#include <array>
#include <vector>

#include "utilities.hpp"

/// A single pendulum in the system.
struct Pendulum {
    Vector2Double position;
    Vector2Double velocity;

    double length = 0.0;
    double mass   = 0.0;

    double angle = 0.0;
    double a_vel = 0.0;
    double a_acc = 0.0;
};

/// Two or more pendulums "welded" or joined with an anchor.
struct WeldedPendulum {
    Color                 color;
    std::vector<Pendulum> pendulums;

    // Trajectories are the end of the final pendulums recorded for each frame
    // Todo: remove frame dependency

    std::array<Vector2Double, 10> trajectories;
    std::size_t                   traj_head = 0;

    /// Update the pendulum.
    void Update();

    /// Draw the pendulum.
    void Draw() const;
};
