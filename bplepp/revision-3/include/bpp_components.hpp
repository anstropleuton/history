/**
 *  @file
 *  @brief      BPP Components.
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
 *  BPP Components provides pre-defined reusable game objects (Components).
 */

#pragma once

#include "PxPhysicsAPI.h" // IWYU pragma: export
#include "bpp_game_object.hpp"
#include "bpp_raylib_ex.hpp"
#include "raylib.h"

using namespace physx;

namespace bpp {

    // Sprite component, draws a sprite
    struct SpriteComponent : GameObject {
        Sprite sprite = {};
        Vector2 origin = { 0.0f, 0.0f };
        float rotation = 0.0f;
        Color tint = WHITE;

        void Init() override;
        void Close() override;
        void Update() override;
        void Draw() const override;
    };

    // Rigid body component, provides dynamic and static rigid body
    struct RigidBodyComponent : GameObject {
        void Init() override;
        void Close() override;
        void Update() override;
        void Draw() const override;

        float &Mass();
        float &Drag();
        float &AngularDrag();
        float &LinearDamping();
        float &AngularDamping();

        Vector2 &CenterOfMass();

        Vector2 &LinearVelocity();
        Vector2 &LinearAcceleration();
        float &AngularVelocity();
        float &AngularAcceleration();

        void ApplyForce(Vector2 force);
        void ApplyTorque(float torque);
    };

    // Collider component
    struct ColliderComponent : GameObject {
        virtual void OnContact(const ColliderComponent &other) {}
        virtual void OnTrigger(const ColliderComponent &other) {}
    };

    struct SquareColliderComponent : ColliderComponent {
        Vector2 size = { 0.0f, 0.0f };

        void Init() override;
        void Close() override;
        void Update() override;
        void Draw() const override;
    };

    struct CircleColliderComponent : ColliderComponent {
        float radius = 0.0f;

        void Init() override;
        void Close() override;
        void Update() override;
        void Draw() const override;
    };

    struct StadiumColliderComponent : ColliderComponent {
        float radius = 0.0f;
        float length = 0.0f;

        void Init() override;
        void Close() override;
        void Update() override;
        void Draw() const override;
    };
} // namespace bpp
