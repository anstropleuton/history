/**
 *  @file
 *  @brief      BPP Engine.
 *  @author     Anstro Pleuton (github.com/anstropleuton)
 *  @copyright  Copyright (c) 2025 Anstro Pleuton
 *
 *  BPP, formally known as Bad Piggies mod Leading Edge Plus Plus (BPLE++,
 *  BPLEPP, BPP), or Bad Piggies Prototype, is a recreation of the mod
 *  Bad Piggies mod Leading Edge in C++.
 *
 *  Due to poor optimizations for the game Bad Piggies, BPLE suffers from
 *  extreme lag as the creation size grows larger. This C++ recreation aims to
 *  fix that issue by using the PhysX physics engine and raylib graphics
 *  library.
 *
 *  The choice for PhysX is simple: to perfectly recreate the physics of the
 *  original game, which is written in Unity.
 *  The choice for raylib is arbitrary, but it is a simple and easy-to-use
 *  library and supports various platforms.
 *
 *  ---
 *
 *  BPP Engine uses PhysX and raylib. Engine data is not exposed to Parts API.
 */

#pragma once

#include "PxPhysicsAPI.h" // IWYU pragma: export

namespace bpp {
} // namespace bpp
