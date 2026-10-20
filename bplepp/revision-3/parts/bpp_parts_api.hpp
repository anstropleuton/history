/**
 *  @file
 *  @brief      BPP Parts API.
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
 *  BPP Parts API allows defining parts for the game.
 */

#pragma once

#include "PxPhysicsAPI.h" // IWYU pragma: export
#include "bpp_game_object.hpp"
#include "bpp_raylib_ex.hpp"
#include "raylib.h"

#include <string>
#include <vector>

using namespace physx;

namespace bpp {

    // API version
    const int BPP_PARTS_API_VERSION = 1;

    // Create, retrieve or modify material
    PxMaterial *&GetMaterial(const std::string &name);

    // Create, retrieve or modify atlas
    Texture2D &GetAtlas(const std::string &name);

    // Create, retrieve or modify sprite
    Sprite &GetSprite(const std::string &name);

    // Game mode
    struct GameMode {
        enum {
            BUILD = 0,
            PLAY = 1
        };
    };

    // Get current game mode
    int GetGameMode();

    // Contraption is a group of connected parts
    struct Contraption; // FWD

    // Part layer
    struct PartLayer {
        enum {
            NORMAL = 1 << 0,
            FRAME = 1 << 1,
            MAX = 1 << 2
        };
    };

    // Part interaction base
    struct PartInteraction {
        virtual ~PartInteraction() = default;
    };

    // Part button interaction
    struct PartButtonInteraction : PartInteraction {
        virtual ~PartButtonInteraction() = default;

        virtual void OnPress() {};
    };

    // Part switch interaction
    struct PartSwitchInteraction : PartInteraction {
        virtual ~PartSwitchInteraction() = default;
        bool value = false;
        bool defaultValue = false;

        // It is the function's job to set the value state
        virtual void OnSwitchToggle() {};
    };

    // Part slider interaction
    struct PartSliderInteraction : PartInteraction {
        virtual ~PartSliderInteraction() = default;
        float value = 0.0f;
        float defaultValue = 0.0f;
        float minValue = 0.0f;
        float maxValue = 1.0f;

        // It is the function's job to set the value state
        virtual void OnValueChange(float newValue) {};
    };

    // A prefab (template) for a part
    struct Part : GameObject {
        virtual ~Part() = default;

        // Automatically set by the game

        Vector2 gridPos = { 0.0f, 0.0f };        // Position in the grid, automatically set

        std::vector<Part *> connectedParts = {}; // Parts this part is directly connected through a joint, automatically set
        Contraption *contraption = nullptr;      // Contraption this part is a part of (get it? part of?), automatically set

        // Part properties

        int layer = PartLayer::NORMAL;             // Layer of the part
        float mass = 1.0f;                         // Mass of the part
        float interactiveRadius = 1.0f;            // Radius of the part (from center) for interaction with touch or mouse click
        float powerConsumption = 0.0f;             // Engine power consumption in the contraption
        float powerProduction = 0.0f;              // Engine power production in the contraption

        Vector2 requiredGridSize = { 1.0f, 1.0f }; // Required grid size for the part, note: integral values only

        // Misc

        std::vector<std::unique_ptr<PartInteraction>> interactions = {}; // Interactions with the part

        // Overridables

        // Override this to allow or disallow joining parts
        virtual bool CanFormJoint(const Part &other) const;

        // Override this to use a different joint type when joining parts
        virtual PxJoint *CreateJoint(const Part &other) const;

        virtual void InitBuildMode();       // Initialize part in build mode
        virtual void CloseBuildMode();      // Close part in build mode
        virtual void UpdateBuildMode();     // Update part in build mode
        virtual void DrawBuildMode() const; // Draw part in build mode

        virtual void InitPlayMode();        // Initialize part in play mode
        virtual void ClosePlayMode();       // Close part in play mode
        virtual void UpdatePlayMode();      // Update part in play mode
        virtual void DrawPlayMode() const;  // Draw part in play mode

        // Unoverridables

        void Init() override;
        void Close() override;
        void Update() override;
        void Draw() const override;
    };

    // Contraption is a group of connected parts
    struct Contraption {
        std::vector<Part> parts;
    };

    // Create, retrieve or modify part
    Part &GetPart(const std::string &type, const std::string &skin);

    // Define this function and return BPP_PARTS_API_VERSION from your code
    extern "C" int BppPartGetVersion();

    // This function is called to initialise your parts
    extern "C" void BppPartInit();

    // This function is called to close your parts, make sure to perform any clean up operations necessary
    extern "C" void BppPartClose();

} // namespace bpp
