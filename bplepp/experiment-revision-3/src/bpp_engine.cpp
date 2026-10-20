/*
 * BPLE++ Engine.
 * Copyright (C) Anstro Pleuton 2025
 * Public Domain or MIT Licensed
 */

#include "bpp_engine.hpp"

#include "PxPhysicsAPI.h" // IWYU pragma: keep
#include "bpp_data.hpp"
#include "bpp_math.hpp"
#include "bpp_misc.hpp"
#include "raylib.h"
#include "raymath.h"

#include <cmath>
#include <queue>
#include <unordered_set>

// Helpers

void AddJoint(bpp::PlayModePart &part, bpp::PlayModePart &otherPart)
{
    const bpp::PartPrefab &prefab = bpp::GetPartPrefab(part.typeID, part.skinID);
    const bpp::PartPrefab &otherPrefab = bpp::GetPartPrefab(otherPart.typeID, otherPart.skinID);

    bpp::Vec2 p1 = part.GetGridPos();
    bpp::Vec2 p2 = otherPart.GetGridPos();
    bpp::Vec2 jointCenter1 = (p2 - p1) / 2.0f;
    bpp::Vec2 jointCenter2 = (p1 - p2) / 2.0f;

    PxFixedJoint *joint = PxFixedJointCreate(
        *bpp::physics,
        part.body,
        PxTransform { jointCenter1.x, jointCenter1.y, 0.0f },
        otherPart.body,
        PxTransform { jointCenter2.x, jointCenter2.y, 0.0f });
    joint->setBreakForce(prefab.jointConnectionStrength + otherPrefab.jointConnectionStrength, INFINITY);

    part.joints.emplace_back(joint);
    part.connectedParts.push_back(&otherPart);
    otherPart.connectedParts.push_back(&part);
}

bpp::Vec2Int bpp::WorldToGrid(Vec2 worldPos)
{
    return { (int)std::round(worldPos.x), (int)std::round(worldPos.y) };
}

bpp::Vec2 bpp::GridToWorld(Vec2Int gridPos)
{
    return Vec2 { (float)gridPos.x, (float)gridPos.y };
}

std::size_t bpp::GetBuildModePartAt(Vec2Int gridPos, int layer)
{
    for (std::size_t i = 0; i < buildModeParts.size(); i++)
    {
        const BuildModePart &part = buildModeParts[i];
        const PartPrefab &prefab = GetPartPrefab(part.typeID, part.skinID);

        if (part.gridPos == gridPos && (prefab.layer & layer) != 0)
        {
            return i;
        }
    }

    return (std::size_t)-1;
}

bool bpp::AddBuildModePartAt(Vec2Int gridPos, BuildModePart part)
{
    const PartPrefab &prefab = GetPartPrefab(part.typeID, part.skinID);

    if (GetBuildModePartAt(gridPos, prefab.layer) != (std::size_t)-1)
    {
        return false;
    }

    buildModeParts.emplace_back(part);
    return true;
}

bool bpp::RemoveBuildModePartAt(Vec2Int gridPos, int layer)
{
    std::size_t part = GetBuildModePartAt(gridPos, layer);

    if (part == (std::size_t)-1)
    {
        return false;
    }

    buildModeParts.erase(buildModeParts.begin() + part);
    return true;
}

bpp::PlayModePart bpp::InitPart(BuildModePart part)
{
    PlayModePart result;
    const PartPrefab &prefab = GetPartPrefab(part.typeID, part.skinID);

    result.typeID = part.typeID;
    result.skinID = part.skinID;

    for (const PartShape &shape : prefab.shapes)
    {
        PxGeometry *geom = shape.geometry.index() == 0
                               ? (PxGeometry *)&std::get<PxBoxGeometry>(shape.geometry)
                           : shape.geometry.index() == 1
                               ? (PxGeometry *)&std::get<PxSphereGeometry>(shape.geometry)
                               : (PxGeometry *)&std::get<PxCapsuleGeometry>(shape.geometry);
        PxShape *pxShape = physics->createShape(*geom, *materials[shape.materialID]);
        pxShape->setLocalPose(shape.transform);
        pxShape->setContactOffset(shape.contactOffset);
        result.shapes.emplace_back(pxShape);
    }

    Vec2 worldPos = GridToWorld(part.gridPos);
    result.body = physics->createRigidDynamic(PxTransform { worldPos.x, worldPos.y, 0.0f });
    result.body->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
    result.body->setMass(prefab.mass);

    for (PxShape *shape : result.shapes)
    {
        result.body->attachShape(*shape);
    }

    scene->addActor(*result.body);

    return result;
}

void bpp::ClosePart(PlayModePart part)
{
    scene->removeActor(*part.body);

    for (PxShape *shape : part.shapes)
    {
        shape->release();
    }

    part.body->release();
}

void bpp::InitBuildMode()
{
}

void bpp::CloseBuildMode()
{
}

void bpp::UpdateBuildMode()
{
    if (IsHotkeyPressed(HOTKEY_ALT | HOTKEY_SHIFT))
    {
        int wheel = GetMouseWheelMove();
        if (wheel > 0)
        {
            ScrollPreviousChosenPartSkin();
        }
        else if (wheel < 0)
        {
            ScrollNextChosenPartSkin();
        }
    }
    else if (IsHotkeyPressed(HOTKEY_ALT))
    {
        int wheel = GetMouseWheelMove();
        if (wheel > 0)
        {
            ScrollPreviousChosenPartType();
        }
        else if (wheel < 0)
        {
            ScrollNextChosenPartType();
        }
    }
    else if (IsHotkeyPressed(HOTKEY_SHIFT))
    {
        int wheel = GetMouseWheelMove();
        if (wheel > 0)
        {
            chosenPartRotation = WrappingMod(chosenPartRotation - 1, 4);
        }
        else if (wheel < 0)
        {
            chosenPartRotation = WrappingMod(chosenPartRotation + 1, 4);
        }
    }
    else if (IsHotkeyPressed(HOTKEY_CONTROL))
    {
        if (IsKeyPressed(KEY_F))
        {
            isChosenPartFlipped = !isChosenPartFlipped;
        }
    }

    float mag = Vec2(GetMouseDelta() / camera.camera.zoom).magnitude();

    for (float f = 0.0f; f <= mag; f++)
    {
        Vec2 pos = GetMousePosition() - GetMouseDelta() * (f / (mag == 0.0f ? 1.0f : mag));
        Vec2 worldPos = camera.GetWorldPosition(pos);
        Vec2Int gridPos = WorldToGrid(worldPos);

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            for (int layer = 1; layer < PartLayer::MAX; layer <<= 1)
            {
                if (RemoveBuildModePartAt(gridPos, layer))
                {
                    break;
                }
            }
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            AddBuildModePartAt(gridPos, GetChosenPart(gridPos));
        }
    }
}

void bpp::DrawBuildMode()
{
    for (int layer = 1; layer < PartLayer::MAX; layer <<= 1)
    {
        for (const BuildModePart &part : buildModeParts)
        {
            const PartPrefab &prefab = GetPartPrefab(part.typeID, part.skinID);

            if ((prefab.layer & layer) == 0)
            {
                continue;
            }

            part.Draw();
        }
    }

    Vec2 worldPos = camera.GetWorldMousePosition();
    Vec2Int gridPos = WorldToGrid(worldPos);
    GetChosenPart(gridPos).Draw(ColorAlpha(WHITE, 0.5f));
}

void bpp::InitPlayMode()
{
    std::vector<PlayModePart> initingPlayModeParts;

    auto GetPlayModePartAt = [&](Vec2Int pos, int layer) -> std::size_t {
        for (std::size_t i = 0; i < initingPlayModeParts.size(); i++)
        {
            const PlayModePart &part = initingPlayModeParts[i];
            const PartPrefab &prefab = GetPartPrefab(part.typeID, part.skinID);

            if (part.GetGridPos() == pos && (prefab.layer & layer) != 0)
            {
                return i;
            }
        }

        return (std::size_t)-1;
    };

    auto GetNeighborPlayModeParts = [&](Vec2Int pos) -> std::vector<std::size_t> {
        std::vector<std::size_t> neighbors;
        std::vector<Vec2Int> offsets = {
            { -1, +0 },
            { +1, -0 },
            { +0, -1 },
            { -0, +1 }
        };

        for (const auto &offset : offsets)
        {
            std::size_t normal = GetPlayModePartAt(pos + offset, PartLayer::NORMAL);

            if (normal != (std::size_t)-1)
            {
                neighbors.emplace_back(normal);
            }

            std::size_t frame = GetPlayModePartAt(pos + offset, PartLayer::FRAME);

            if (frame != (std::size_t)-1)
            {
                neighbors.emplace_back(frame);
            }
        }

        return neighbors;
    };

    for (const BuildModePart &part : buildModeParts)
    {
        initingPlayModeParts.emplace_back(InitPart(part));
    }

    // Add joints
    for (PlayModePart &part : initingPlayModeParts)
    {
        const PartPrefab &prefab = GetPartPrefab(part.typeID, part.skinID);
        if (prefab.jointConnectionType != PartPrefab::JointConnectionType::SOURCE)
        {
            continue;
        }

        for (std::size_t &neighbor : GetNeighborPlayModeParts(part.GetGridPos()))
        {
            PlayModePart &neighborPart = initingPlayModeParts[neighbor];
            const PartPrefab &neighborPrefab = GetPartPrefab(neighborPart.typeID, neighborPart.skinID);

            if (neighborPrefab.jointConnectionType == PartPrefab::JointConnectionType::NONE)
            {
                continue;
            }

            AddJoint(part, neighborPart);
        }

        // Add joints to parts in same position but in other layer
        for (int layer = 1; layer < PartLayer::MAX; layer <<= 1)
        {
            if (layer == prefab.layer)
            {
                continue;
            }

            std::size_t other = GetPlayModePartAt(part.GetGridPos(), layer);

            if (other == (std::size_t)-1)
            {
                continue;
            }

            PlayModePart &otherPart = initingPlayModeParts[other];
            const PartPrefab &otherPrefab = GetPartPrefab(otherPart.typeID, otherPart.skinID);

            if (otherPrefab.jointConnectionType == PartPrefab::JointConnectionType::NONE)
            {
                continue;
            }

            AddJoint(part, otherPart);
        }
    }

    std::unordered_set<std::size_t> visited;

    auto CollectConnectedParts = [&](std::size_t startIndex) -> std::vector<std::size_t> {
        std::vector<std::size_t> group;
        std::queue<std::size_t> toVisit;

        toVisit.push(startIndex);
        visited.insert(startIndex);

        while (!toVisit.empty())
        {
            std::size_t current = toVisit.front();
            toVisit.pop();

            group.emplace_back(current);
            PlayModePart &currentPart = initingPlayModeParts[current];

            for (PlayModePart *connectedPart : currentPart.connectedParts)
            {
                std::size_t connectedIndex = std::distance(&initingPlayModeParts[0], connectedPart);

                if (visited.find(connectedIndex) == visited.end())
                {
                    visited.insert(connectedIndex);
                    toVisit.push(connectedIndex);
                }
            }
        }

        return group;
    };

    for (std::size_t i = 0; i < initingPlayModeParts.size(); i++)
    {
        if (visited.find(i) != visited.end())
        {
            continue;
        }

        std::vector<std::size_t> connectedParts = CollectConnectedParts(i);

        Contraption contraption;
        for (std::size_t partIndex : connectedParts)
        {
            contraption.parts.push_back(initingPlayModeParts[partIndex]);
        }

        contraptions.push_back(std::move(contraption));
    }
}

void bpp::ClosePlayMode()
{
    for (Contraption &contraption : contraptions)
    {
        for (PlayModePart &part : contraption.parts)
        {
            for (PxJoint *joint : part.joints)
            {
                joint->release();
            }

            ClosePart(part);
        }
    }

    contraptions.clear();
}

void bpp::UpdatePlayMode()
{
    scene->simulate(1.0f / 60.0f);
    scene->fetchResults(true);
}

void bpp::DrawPlayMode()
{
    for (std::size_t i = 0; i < contraptions.size(); i++)
    {
        const Contraption &contraption = contraptions[i];
        Color debugColor = ColorFromHSV(360.0f * i / contraptions.size(), 1.0f, 1.0f);

        for (int layer = 1; layer < PartLayer::MAX; layer <<= 1)
        {
            for (const PlayModePart &part : contraption.parts)
            {
                const PartPrefab &prefab = GetPartPrefab(part.typeID, part.skinID);

                if ((prefab.layer & layer) == 0)
                {
                    continue;
                }

                part.Draw(drawDebug ? debugColor : WHITE);
            }
        }
    }
}

void EngineInitRaylib()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(960, 540, "Untitled");
    SetTargetFPS(60);
}

void EngineInitPhysX()
{
    bpp::foundation = PxCreateFoundation(PX_PHYSICS_VERSION, bpp::allocator, bpp::errorCallback);
    bpp::physics = PxCreatePhysics(PX_PHYSICS_VERSION, *bpp::foundation, PxTolerancesScale {});

    PxSceneDesc sceneDesc = bpp::physics->getTolerancesScale();
    sceneDesc.gravity = { 0.0f, 9.81f, 0.0f };
    sceneDesc.cpuDispatcher = bpp::dispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    bpp::scene = bpp::physics->createScene(sceneDesc);

    bpp::groundMaterial = bpp::physics->createMaterial(1.0f, 1.0f, 0.0f);

    auto *plane = PxCreatePlane(*bpp::physics, PxPlane { 0.0f, -1.0f, 0.0f, 0.5f }, *bpp::groundMaterial);
    bpp::scene->addActor(*plane);
}

void EngineCloseRaylib()
{
    CloseWindow();
}

void EngineClosePhysX()
{
    bpp::groundMaterial->release();
    bpp::scene->release();
    bpp::dispatcher->release();
    bpp::physics->release();
    bpp::foundation->release();
}

void bpp::InitEngine()
{
    EngineInitRaylib();
    EngineInitPhysX();

    DataLoadAll();
}

void bpp::CloseEngine()
{
    DataUnloadAll();

    EngineClosePhysX();
    EngineCloseRaylib();
}

void bpp::UpdateEngine()
{
    camera.update();

    if (IsKeyPressed(KEY_ENTER))
    {
        SwitchGameMode();
    }

    if (gameMode == GameMode::BUILD)
    {
        UpdateBuildMode();
    }
    else
    {
        UpdatePlayMode();
    }
}

void bpp::DrawEngine()
{
    camera.BeginMode2D();

    if (gameMode == GameMode::BUILD)
    {
        DrawBuildMode();
    }
    else
    {
        DrawPlayMode();
    }

    camera.EndMode2D();
}
