/**
 *  @file
 *  @brief      BPP Game Object.
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
 *  BPP Game Object defines game object structure. The Game Object has 4
 *  fundamental duties:
 *  - Hierarchy structure
 *  - Transformation management
 *  - Lifecycle management
 *  - Object querying
 */

#pragma once

#include "PxPhysicsAPI.h" // IWYU pragma: export
#include "raylib.h"

#include <memory>
#include <vector>

using namespace physx;

namespace bpp {

    // Game Object Transformation
    struct Transform {
        Vector2 position = { 0.0f, 0.0f };
        float rotation = 0.0f; // In degrees, from center
        Vector2 scale = { 1.0f, 1.0f };

        Transform() = default;

        // Convert PhysX transform to Transform
        // Note: PhysX's PxTransform does not handle scaling, set scale to default
        Transform(PxTransform pxTransform)
            : position({ pxTransform.p.x, pxTransform.p.y }),
              rotation(pxTransform.q.getAngle() * RAD2DEG),
              scale({ 1.0f, 1.0f }) {}

        // Convert Rectangle to Transform
        // Note: raylib's Rectangle does not handle rotation, set rotation to default
        Transform(Rectangle rec)
            : position({ rec.x + rec.width / 2, rec.y + rec.height / 2 }),
              rotation(0.0f),
              scale({ rec.width, rec.height }) {}

        // Convert Transform to PhysX transform
        // Note: PhysX's PxTransform does not handle scaling, discard scaling
        operator PxTransform() const
        {
            return PxTransform({ position.x, position.y, 0.0f }, PxQuat(rotation * DEG2RAD, { 0.0f, 0.0f, 1.0f }));
        }

        // Convert Transform to Rectangle
        // Note: raylib's Rectangle does not handle rotation, discard rotation
        operator Rectangle() const
        {
            return { position.x - scale.x / 2, position.y - scale.y / 2, scale.x, scale.y };
        }
    };

    // Game Object
    struct GameObject {
        virtual ~GameObject() = default;

        Transform transform;

        // Get transformation from the root
        Transform GetWorldTransform() const;

        // Hierarchy structure

        std::vector<std::unique_ptr<GameObject>> children;
        std::weak_ptr<GameObject> parent;

        // Get children and grandchildren of a specific type
        template <typename T>
        std::vector<std::weak_ptr<T>> GetChildrenOfType() const
        {
            std::vector<std::weak_ptr<T>> results;

            for (const auto &child : children)
            {
                if (auto casted = std::dynamic_pointer_cast<T>(child))
                {
                    results.push_back(casted);
                }

                auto childResults = child->GetChildrenOfType<T>();
                results.insert(results.end(), childResults.begin(), childResults.end());
            }

            return results;
        }

        // Get parent or grandparent of a specific type
        template <typename T>
        std::weak_ptr<T> GetParentOfType() const
        {
            auto current = parent.lock();
            if (!current)
            {
                return nullptr;
            }

            if (auto casted = std::dynamic_pointer_cast<T>(current))
            {
                return casted;
            }

            return current->GetParentOfType<T>();
        }

        // Common overridables

        // Do not initialize physics in Init, it is done automatically
        virtual void Init() {}

        // Do not close physics in Close, it is done automatically
        virtual void Close() {}

        // It is advised to not use GetChildrenOfType or GetParentOfType doe to performance reasons
        virtual void Update() {}

        // It is advised to not use GetChildrenOfType or GetParentOfType doe to performance reasons
        virtual void Draw() const {}
    };
} // namespace bpp