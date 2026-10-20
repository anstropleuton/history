/*
 * BPLE++ Engine.
 * Copyright (C) Anstro Pleuton 2025
 * Public Domain or MIT Licensed
 */

#pragma once

#include "PxPhysicsAPI.h" // IWYU pragma: keep
#include "bpp_camera.hpp"
#include "bpp_data.hpp"
#include "bpp_math.hpp"
#include "foundation/PxVec3.h"
#include "raylib.h"

#include <string>
#include <vector>

#ifndef global
#define global extern
#endif

namespace bpp {

    const bool drawDebug = true;

    Vec2Int WorldToGrid(Vec2 worldPos);
    Vec2 GridToWorld(Vec2Int gridPos);

    struct BuildModePart {
        std::string typeID = "";
        std::string skinID = "";

        Vec2Int gridPos = { 0, 0 };
        int gridRotation = 0;
        bool isFlipped = false;

        float GetRotation() const
        {
            switch (gridRotation)
            {
            case 1: return 90.0f;
            case 2: return 180.0f;
            case 3: return 270.0f;
            case 4: return 45.0f;
            case 5: return 135.0f;
            case 6: return 225.0f;
            case 7: return 315.0f;
            default: return 0.0f;
            }
        }

        void Draw(BppColor tint = WHITE) const
        {
            for (const PartSprite &partSprite : GetPartPrefab(typeID, skinID).sprites)
            {
                partSprite.Draw(gridPos, GetRotation(), tint);
            }
        }
    };

    global std::vector<BuildModePart> buildModeParts;

    std::size_t GetBuildModePartAt(Vec2Int gridPos, int layer);
    bool AddBuildModePartAt(Vec2Int gridPos, BuildModePart part);
    bool RemoveBuildModePartAt(Vec2Int gridPos, int layer);

    struct Contraption; // FWD

    struct PlayModePart {
        std::string typeID = "";
        std::string skinID = "";
        std::vector<PxShape *> shapes = {};
        PxRigidDynamic *body = nullptr;
        std::vector<PxJoint *> joints = {};

        std::vector<PlayModePart *> connectedParts;

        Contraption *contraption;

        Vec2Int GetGridPos() const
        {
            return { (int)std::round(body->getGlobalPose().p.x), (int)std::round(body->getGlobalPose().p.y) };
        }

        void Draw(BppColor tint = WHITE) const
        {
            PxTransform pose = body->getGlobalPose();

            float angleRad = 0.0f;
            PxVec3 axis = {};
            pose.q.toRadiansAndUnitAxis(angleRad, axis);
            angleRad *= axis.dot(PxVec3(0.0f, 0.0f, 1.0f));

            for (const PartSprite &partSprite : GetPartPrefab(typeID, skinID).sprites)
            {
                partSprite.Draw({ pose.p.x, pose.p.y }, angleRad * RAD2DEG, tint);
            }
        }
    };

    struct Contraption {
        std::vector<PlayModePart> parts;
    };

    global std::vector<Contraption> contraptions;

    PlayModePart InitPart(BuildModePart part);
    void ClosePart(PlayModePart part);

    struct GameMode {
        enum {
            BUILD,
            PLAY
        };
    };

    global int gameMode;

    void InitBuildMode();
    void CloseBuildMode();
    void UpdateBuildMode();
    void DrawBuildMode();
    void InitPlayMode();
    void ClosePlayMode();
    void UpdatePlayMode();
    void DrawPlayMode();

    inline void SwitchGameMode()
    {
        if (gameMode == GameMode::BUILD)
        {
            CloseBuildMode();
            gameMode = GameMode::PLAY;
            InitPlayMode();
        }
        else
        {
            ClosePlayMode();
            gameMode = GameMode::BUILD;
            InitBuildMode();
        }
    }

    global PxDefaultAllocator allocator;
    global PxDefaultErrorCallback errorCallback;
    global PxFoundation *foundation;
    global PxPhysics *physics;
    global PxDefaultCpuDispatcher *dispatcher;
    global PxScene *scene;
    global PxMaterial *groundMaterial;
    global WorldCamera2D camera;

    global std::size_t chosenPartTypeIndex;
    global std::size_t chosenPartSkinIndex;
    global int chosenPartRotation;
    global bool isChosenPartFlipped;

    inline void ScrollNextChosenPartType()
    {
        chosenPartTypeIndex = (chosenPartTypeIndex + 1) % parts.size();
        chosenPartSkinIndex = 0;
    }

    inline void ScrollPreviousChosenPartType()
    {
        chosenPartTypeIndex = (chosenPartTypeIndex + parts.size() - 1) % parts.size();
        chosenPartSkinIndex = 0;
    }

    inline void ScrollNextChosenPartSkin()
    {
        chosenPartSkinIndex = (chosenPartSkinIndex + 1) % parts[chosenPartTypeIndex].skins.size();
    }

    inline void ScrollPreviousChosenPartSkin()
    {
        chosenPartSkinIndex = (chosenPartSkinIndex + parts[chosenPartTypeIndex].skins.size() - 1) % parts[chosenPartTypeIndex].skins.size();
    }

    inline void ScrollNextChosenPartRotation()
    {
        chosenPartRotation = (chosenPartRotation + 1) % 8;
    }

    inline void ScrollPreviousChosenPartRotation()
    {
        chosenPartRotation = (chosenPartRotation + 7) % 8;
    }

    inline void FlipChosenPart()
    {
        isChosenPartFlipped = !isChosenPartFlipped;
    }

    inline BuildModePart GetChosenPart(Vec2Int gridPos)
    {
        return {
            .typeID = parts[chosenPartTypeIndex].typeID,
            .skinID = parts[chosenPartTypeIndex].skins[chosenPartSkinIndex].skinID,
            .gridPos = gridPos,
            .gridRotation = chosenPartRotation,
            .isFlipped = isChosenPartFlipped
        };
    }

    void InitEngine();
    void CloseEngine();
    void UpdateEngine();
    void DrawEngine();
} // namespace bpp
