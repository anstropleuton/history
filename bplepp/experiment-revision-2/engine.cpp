#include <random>

#define link
#include "engine.hpp"

bool IsHotkeyPressed(int hotkey)
{
    bool result { true };
    result &= ((hotkey & HOTKEY_SHIFT) != 0) == (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
    result &= ((hotkey & HOTKEY_CONTROL) != 0) == (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL));
    result &= ((hotkey & HOTKEY_ALT) != 0) == (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT));
    result &= ((hotkey & HOTKEY_SUPER) != 0) == (IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER));
    return result;
}

Vec2 GetPreviousMousePosition()
{
    Vec2 position { GetMousePosition() };
    Vec2 delta { GetMouseDelta() };
    return position - delta;
}

int SeededRandomInt(int min, int max)
{
    static std::mt19937 mt { 0 };
    std::uniform_int_distribution<int> dist { min, max };
    return dist(mt);
}

int AbsMod(int a, int b)
{
    return (a > 0) ? a % b : (b + a) % b;
}

void EngineLoadMaterials()
{
    materials[MaterialID::CONTRAPTION] = physics->createMaterial(0.7f, 0.5f, 0.0f);
    materials[MaterialID::CONTRAPTION]->setFrictionCombineMode(PxCombineMode::eAVERAGE);
    materials[MaterialID::CONTRAPTION]->setRestitutionCombineMode(PxCombineMode::eAVERAGE);
    materials[MaterialID::CONTRAPTION_ALIEN] = physics->createMaterial(0.0f, 0.0f, 0.0f);
    materials[MaterialID::CONTRAPTION_ALIEN]->setFrictionCombineMode(PxCombineMode::eAVERAGE);
    materials[MaterialID::CONTRAPTION_ALIEN]->setRestitutionCombineMode(PxCombineMode::eAVERAGE);
    materials[MaterialID::CONTRAPTION_LOW_FRICTION] = physics->createMaterial(0.0f, 0.0f, 0.0f);
    materials[MaterialID::CONTRAPTION_LOW_FRICTION]->setFrictionCombineMode(PxCombineMode::eMULTIPLY);
    materials[MaterialID::CONTRAPTION_LOW_FRICTION]->setRestitutionCombineMode(PxCombineMode::eAVERAGE);
    materials[MaterialID::CONTRAPTION_WHEEL_FRICTION] = physics->createMaterial(0.025f, 0.0f, 0.0f);
    materials[MaterialID::CONTRAPTION_WHEEL_FRICTION]->setFrictionCombineMode(PxCombineMode::eMIN);
    materials[MaterialID::CONTRAPTION_WHEEL_FRICTION]->setRestitutionCombineMode(PxCombineMode::eMIN);
    materials[MaterialID::CONTRAPTION_WOODEN_WHEEL_FRICTION] = physics->createMaterial(0.05f, 0.0f, 0.0f);
    materials[MaterialID::CONTRAPTION_WOODEN_WHEEL_FRICTION]->setFrictionCombineMode(PxCombineMode::eMIN);
    materials[MaterialID::CONTRAPTION_WOODEN_WHEEL_FRICTION]->setRestitutionCombineMode(PxCombineMode::eMIN);
}

void EngineUnloadMaterials()
{
    materials[MaterialID::CONTRAPTION]->release();
    materials[MaterialID::CONTRAPTION_ALIEN]->release();
    materials[MaterialID::CONTRAPTION_LOW_FRICTION]->release();
    materials[MaterialID::CONTRAPTION_WHEEL_FRICTION]->release();
    materials[MaterialID::CONTRAPTION_WOODEN_WHEEL_FRICTION]->release();
}

void EngineLoadAtlases()
{
    atlases[AtlasID::INGAME_ATLAS] = LoadTexture(RESOURCES_PATH "/IngameAtlas.png");
    SetTextureFilter(atlases[AtlasID::INGAME_ATLAS], TEXTURE_FILTER_TRILINEAR);
    atlases[AtlasID::INGAME_ATLAS_2] = LoadTexture(RESOURCES_PATH "/IngameAtlas2.png");
    SetTextureFilter(atlases[AtlasID::INGAME_ATLAS_2], TEXTURE_FILTER_TRILINEAR);
    atlases[AtlasID::INGAME_ATLAS_3] = LoadTexture(RESOURCES_PATH "/IngameAtlas3.png");
    SetTextureFilter(atlases[AtlasID::INGAME_ATLAS_3], TEXTURE_FILTER_TRILINEAR);
    atlases[AtlasID::INGAME_ATLAS_4] = LoadTexture(RESOURCES_PATH "/IngameAtlas4.png");
    SetTextureFilter(atlases[AtlasID::INGAME_ATLAS_4], TEXTURE_FILTER_TRILINEAR);
    atlases[AtlasID::MENU_ATLAS] = LoadTexture(RESOURCES_PATH "/MenuAtlas.png");
    SetTextureFilter(atlases[AtlasID::MENU_ATLAS], TEXTURE_FILTER_TRILINEAR);
    atlases[AtlasID::MENU_ATLAS_2] = LoadTexture(RESOURCES_PATH "/MenuAtlas2.png");
    SetTextureFilter(atlases[AtlasID::MENU_ATLAS_2], TEXTURE_FILTER_TRILINEAR);
    atlases[AtlasID::MENU_ATLAS_3] = LoadTexture(RESOURCES_PATH "/MenuAtlas3.png");
    SetTextureFilter(atlases[AtlasID::MENU_ATLAS_3], TEXTURE_FILTER_TRILINEAR);
}

void EngineUnloadAtlases()
{
    UnloadTexture(atlases[AtlasID::INGAME_ATLAS]);
    UnloadTexture(atlases[AtlasID::INGAME_ATLAS_2]);
    UnloadTexture(atlases[AtlasID::INGAME_ATLAS_3]);
    UnloadTexture(atlases[AtlasID::INGAME_ATLAS_4]);
    UnloadTexture(atlases[AtlasID::MENU_ATLAS]);
    UnloadTexture(atlases[AtlasID::MENU_ATLAS_2]);
    UnloadTexture(atlases[AtlasID::MENU_ATLAS_3]);
}

void EngineLoadSprites()
{
    sprites[SpriteID::EGG] = { AtlasID::INGAME_ATLAS, Rect { 808.0001f, 5.00013f, 80.0f, 81.99999f } };
    sprites[SpriteID::WOODEN_FRAME] = { AtlasID::INGAME_ATLAS, Rect { 680.9999f, 520.0001f, 105.0f, 106.0f } };
}

void EngineUnloadSprites() {}

void EngineLoadPrefabs()
{
    partPrefabs[PartPrefabID::EGG] = { { PartShape { MaterialID::CONTRAPTION, PxCapsuleGeometry { 0.3f, 0.85f / 2.0f - 0.3f },
                                                     PxTransform { PxVec3 { 0.0f, -0.02f, 0.0f }, PxQuat { 0.0f, 0.0f, -0.3826835f, 0.9238796f } }, 0.005f } },
                                       { PartSprite { SpriteID::EGG, Rect { 0.0f, 0.0f, 0.8333333f, 0.8333333f }, Vec2 { 0.4166667f, 0.4166667f }, 0.0f,
                                                      WHITE } },
                                       0.5f,
                                       false,
                                       PartPrefab::JointConnectionType::NONE };

    partPrefabs[PartPrefabID::WOODEN_FRAME] = {
        { PartShape { MaterialID::CONTRAPTION, PxBoxGeometry { 0.5f, 0.5f, 0.5f }, PxTransform { PxIdentity }, 0.005f } },
        { PartSprite { SpriteID::WOODEN_FRAME, Rect { 0.0f, 0.0f, 1.09375f, 1.09375f }, Vec2 { 0.546875f, 0.546875f }, 0.0f, WHITE } },
        0.5f,
        true,
        PartPrefab::JointConnectionType::SOURCE
    };
}

void EngineUnloadPrefabs() {}

void EngineLoadData()
{
    EngineLoadMaterials();
    EngineLoadAtlases();
    EngineLoadSprites();
    EngineLoadPrefabs();
}

void EngineUnloadData()
{
    EngineUnloadMaterials();
    EngineUnloadAtlases();
    EngineUnloadSprites();
    EngineUnloadPrefabs();
}

void InitBuildMode()
{
    currentGameMode = GameMode::BUILD_MODE;
}

void CloseBuildMode() {}

void InitPlayMode()
{
    for (auto part : buildModeParts)
    {
        playModeParts.push_back(InitPart(part));
    }

    auto getNeighborParts { [](Vec2Int position) -> std::array<std::size_t, 8> {
        return { FindPlayModePartAt({ position.x, position.y + 1 }, false), FindPlayModePartAt({ position.x, position.y - 1 }, false), FindPlayModePartAt({ position.x + 1, position.y }, false),
                 FindPlayModePartAt({ position.x - 1, position.y }, false), FindPlayModePartAt({ position.x, position.y + 1 }, true),  FindPlayModePartAt({ position.x, position.y - 1 }, true),
                 FindPlayModePartAt({ position.x + 1, position.y }, true),  FindPlayModePartAt({ position.x - 1, position.y }, true) };
    } };

    for (auto &part : playModeParts)
    {
        auto prefab { partPrefabs[part.prefabId] };
        if (prefab.connectionType != PartPrefab::JointConnectionType::SOURCE)
        {
            continue;
        }

        auto foundNeighborParts { getNeighborParts(part.getIntPosition()) };
        for (auto i { 0zu }; i < foundNeighborParts.size(); i++)
        {
            auto foundNeighborPart { foundNeighborParts[i] };
            if (foundNeighborPart != npos)
            {
                auto neighborPart { playModeParts[foundNeighborPart] };
                auto neighborPartPrefab { partPrefabs[neighborPart.prefabId] };
                if (neighborPartPrefab.connectionType == PartPrefab::JointConnectionType::NONE)
                {
                    continue;
                }

                auto p1 { part.body->getGlobalPose().p };
                auto p2 { neighborPart.body->getGlobalPose().p };
                Vec3 jointCenter1 { (p2 - p1) / 2.0f };
                Vec3 jointCenter2 { (p1 - p2) / 2.0f };
                part.joints.push_back(PxFixedJointCreate(*physics, part.body, PxTransform { jointCenter1 }, neighborPart.body, PxTransform { jointCenter2 }));
            }
        }

        auto foundOther { FindPlayModePartAt(part.getIntPosition(), !partPrefabs[part.prefabId].isFrame) };
        if (foundOther != npos)
        {
            auto other { playModeParts[foundOther] };
            part.joints.push_back(PxFixedJointCreate(*physics, part.body, PxTransform { PxIdentity }, other.body, PxTransform { PxIdentity }));
        }
    }

    currentGameMode = GameMode::PLAY_MODE;
}

void ClosePlayMode()
{
    for (auto part : playModeParts)
    {
        for (auto joint : part.joints)
        {
            if (joint)
            {
                joint->release();
            }
        }
        ClosePart(part);
    }
    playModeParts.clear();
}

void SwitchGameMode()
{
    if (currentGameMode == GameMode::BUILD_MODE)
    {
        CloseBuildMode();
        InitPlayMode();
    }
    else if (currentGameMode == GameMode::PLAY_MODE)
    {
        ClosePlayMode();
        InitBuildMode();
    }
}

float BuildModePart::rotationDegreesFromGridRotation(int gridRotation)
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

void BuildModePart::draw(CColor tint)
{
    auto prefab { partPrefabs[prefabId] };
    for (auto partSprite : prefab.partSprites)
    {
        auto sprite { sprites[partSprite.spriteId] };
        auto dest { partSprite.dest };
        dest.p(GridToWorld(dest.p() + position));
        DrawTexturePro(atlases[sprite.atlasId], sprite.src, dest, partSprite.origin,
                       rotationDegreesFromGridRotation(gridRotation) + partSprite.rotationOffsetDegrees, ColorTint(partSprite.tint, tint));
    }
}

void PlayModePart::draw(CColor tint)
{
    auto pose { body->getGlobalPose() };

    float angleRadians {};
    PxVec3 axis {};
    pose.q.toRadiansAndUnitAxis(angleRadians, axis);
    angleRadians *= axis.dot(PxVec3(0.0f, 0.0f, 1.0f));

    auto prefab { partPrefabs[prefabId] };
    for (auto partSprite : prefab.partSprites)
    {
        auto sprite { sprites[partSprite.spriteId] };
        auto dest { partSprite.dest };
        dest.p(dest.p() + Vec2 { pose.p.x, pose.p.y });
        DrawTexturePro(atlases[sprite.atlasId], sprite.src, dest, partSprite.origin, angleRadians * RAD2DEG + partSprite.rotationOffsetDegrees,
                       ColorTint(partSprite.tint, tint));
    }
}

Vec2Int WorldToGrid(Vec2 position)
{
    return { (int)std::round(position.x), (int)std::round(-position.y) };
}

Vec2 GridToWorld(Vec2Int position)
{
    return Vec2Int { position.x, -position.y };
}

Vec2Int Rounded(Vec2 position)
{
    return { (int)std::round(position.x), (int)std::round(position.y) };
}

std::size_t FindBuildModePartAt(Vec2Int position, bool frame)
{
    for (auto i { buildModeParts.size() }; i-- > 0;)
    {
        auto part { buildModeParts[i] };
        if (part.position == position && partPrefabs[part.prefabId].isFrame == frame)
        {
            return i;
        }
    }
    return npos;
}

std::size_t FindPlayModePartAt(Vec2Int position, bool frame)
{
    for (auto i { playModeParts.size() }; i-- > 0;)
    {
        auto part { playModeParts[i] };
        if (part.getIntPosition() == position && partPrefabs[part.prefabId].isFrame == frame)
        {
            return i;
        }
    }
    return npos;
}

bool AddPartAtPos(Vec2Int position, BuildModePart part)
{
    part.position = position;
    auto foundPart { FindBuildModePartAt(position, partPrefabs[part.prefabId].isFrame) };
    if (foundPart == npos)
    {
        buildModeParts.push_back(part);
        return true;
    }
    return false;
}

bool RemovePartAtPos(Vec2Int position, bool frame)
{
    auto foundPart { FindBuildModePartAt(position, frame) };
    if (foundPart != npos)
    {
        buildModeParts.erase(buildModeParts.begin() + foundPart);
        return true;
    }
    return false;
}

PlayModePart InitPart(BuildModePart buildPart)
{
    PlayModePart playPart { buildPart.prefabId };
    auto &prefab { partPrefabs[buildPart.prefabId] };

    for (auto &partShape : prefab.partShapes)
    {
        PxGeometry *geom {};
        if ((geom = std::get_if<PxSphereGeometry>(&partShape.geometry)))
        {
        }
        else if ((geom = std::get_if<PxCapsuleGeometry>(&partShape.geometry)))
        {
        }
        else if ((geom = std::get_if<PxBoxGeometry>(&partShape.geometry)))
        {
        }
        else
        {
            break;
        }

        auto shape { physics->createShape(*geom, *materials[partShape.materialId]) };
        shape->setLocalPose(partShape.transform);
        shape->setContactOffset(partShape.contactOffset);
        playPart.shapes.push_back(shape);
    }

    Vec2 worldPosition { GridToWorld(buildPart.position) };
    playPart.body = physics->createRigidDynamic(PxTransform { { worldPosition.x, worldPosition.y, 0.0f } });
    playPart.body->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
                                            PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
    PxRigidBodyExt::updateMassAndInertia(*playPart.body, prefab.mass);

    for (auto shape : playPart.shapes)
    {
        playPart.body->attachShape(*shape);
    }
    scene->addActor(*playPart.body);

    return playPart;
}

void ClosePart(PlayModePart playPart)
{
    scene->removeActor(*playPart.body);
    for (auto shape : playPart.shapes)
    {
        if (shape)
        {
            shape->release();
        }
    }
    if (playPart.body)
    {
        playPart.body->release();
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
    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
    physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale {});

    PxSceneDesc sceneDesc { physics->getTolerancesScale() };
    sceneDesc.gravity = { 0.0f, 9.81f, 0.0f };
    sceneDesc.cpuDispatcher = dispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    scene = physics->createScene(sceneDesc);

    groundMaterial = physics->createMaterial(1.0f, 1.0f, 0.0f);

    auto *plane { PxCreatePlane(*physics, PxPlane { 0.0f, -1.0f, 0.0f, terrainOffset.y }, *groundMaterial) };
    scene->addActor(*plane);
    terrain.init();
}

void EngineCloseRaylib()
{
    CloseWindow();
}

void EngineClosePhysX()
{
    terrain.close();
    groundMaterial->release();
    scene->release();
    dispatcher->release();
    physics->release();
    foundation->release();
}

void EngineInit()
{
    EngineInitRaylib();
    EngineInitPhysX();

    EngineLoadData();
    InitBuildMode();
}

void EngineClose()
{
    if (currentGameMode == GameMode::BUILD_MODE) CloseBuildMode();
    if (currentGameMode == GameMode::PLAY_MODE) ClosePlayMode();
    EngineUnloadData();

    EngineClosePhysX();
    EngineCloseRaylib();
}

void EngineUpdate()
{
    if (currentGameMode == GameMode::PLAY_MODE)
    {
        scene->simulate(1.0f / 60.0f);
        scene->fetchResults(true);
    }
}

void EngineDraw()
{
    if (currentGameMode == GameMode::BUILD_MODE)
    {
        for (auto part : buildModeParts)
        {
            part.draw();
        }
    }
    else if (currentGameMode == GameMode::PLAY_MODE)
    {
        for (auto part : playModeParts)
        {
            part.draw();
        }
    }
}
