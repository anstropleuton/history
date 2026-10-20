/*
 * BPLE++ Data.
 * Copyright (C) Anstro Pleuton 2025
 * Public Domain or MIT Licensed
 */

#include "PxPhysicsAPI.h" // IWYU pragma: keep
#include "bpp_data.hpp"

#include "bpp_engine.hpp"

#ifndef DATA_RESOURCES_PATH
#define DATA_RESOURCES_PATH "res"
#endif

void bpp::DataLoadMaterials()
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

void bpp::DataUnloadMaterials()
{
    for (auto &pair : materials)
    {
        pair.second->release();
    }

    materials.clear();
}

void bpp::DataLoadAtlases()
{
    atlases[AtlasID::INGAME_ATLAS] = LoadTexture(DATA_RESOURCES_PATH "/IngameAtlas.png");
    atlases[AtlasID::INGAME_ATLAS_2] = LoadTexture(DATA_RESOURCES_PATH "/IngameAtlas2.png");
    atlases[AtlasID::INGAME_ATLAS_3] = LoadTexture(DATA_RESOURCES_PATH "/IngameAtlas3.png");
    atlases[AtlasID::INGAME_ATLAS_4] = LoadTexture(DATA_RESOURCES_PATH "/IngameAtlas4.png");
    atlases[AtlasID::MENU_ATLAS] = LoadTexture(DATA_RESOURCES_PATH "/MenuAtlas.png");
    atlases[AtlasID::MENU_ATLAS_2] = LoadTexture(DATA_RESOURCES_PATH "/MenuAtlas2.png");
    atlases[AtlasID::MENU_ATLAS_3] = LoadTexture(DATA_RESOURCES_PATH "/MenuAtlas3.png");
}

void bpp::DataUnloadAtlases()
{
    for (auto &pair : atlases)
    {
        UnloadTexture(pair.second);
    }

    atlases.clear();
}

void bpp::DataLoadSprites()
{
    sprites[SpriteID::EGG_REGULAR] = { AtlasID::INGAME_ATLAS, Rect { 808.0001f, 5.00013f, 80.0f, 81.99999f } };
    sprites[SpriteID::WOODEN_FRAME_REGULAR] = { AtlasID::INGAME_ATLAS, Rect { 680.9999f, 520.0001f, 105.0f, 106.0f } };
}

void bpp::DataUnloadSprites()
{
    sprites.clear();
}

void bpp::DataLoadPartPrefabs()
{
    const PartPrefab EGG_REGULAR = {
        .shapes = { { MaterialID::CONTRAPTION, PxCapsuleGeometry { 0.3f, 0.85f / 2.0f - 0.3f }, PxTransform { PxVec3 { 0.0f, -0.02f, 0.0f }, PxQuat { 0.0f, 0.0f, -0.3826835f, 0.9238796f } }, 0.005f } },
        .sprites = { { SpriteID::EGG_REGULAR, Rect { 0.0f, 0.0f, 0.8333333f, 0.8333333f }, Vec2 { 0.4166667f, 0.4166667f }, 0.0f, WHITE } },
        .mass = 0.5f,
        .layer = PartLayer::NORMAL,
        .jointConnectionType = PartPrefab::JointConnectionType::NONE,
        .jointConnectionStrength = 125.0f
    };
    AddPartPrefab("EGG", "REGULAR", EGG_REGULAR);

    const PartPrefab WOODEN_FRAME_REGULAR = {
        .shapes = { { MaterialID::CONTRAPTION, PxBoxGeometry { 0.5f, 0.5f, 0.5f }, PxTransform { PxIdentity }, 0.005f } },
        .sprites = { { SpriteID::WOODEN_FRAME_REGULAR, Rect { 0.0f, 0.0f, 1.0f, 1.0f }, Vec2 { 0.5f, 0.5f }, 0.0f, WHITE } },
        .mass = 0.5f,
        .layer = PartLayer::FRAME,
        .jointConnectionType = PartPrefab::JointConnectionType::SOURCE,
        .jointConnectionStrength = 125.0f
    };
    AddPartPrefab("WOODEN_FRAME", "REGULAR", WOODEN_FRAME_REGULAR);
}

void bpp::DataUnloadPartPrefabs()
{
    parts.clear();
}

void bpp::DataLoadAll()
{
    DataLoadMaterials();
    DataLoadAtlases();
    DataLoadSprites();
    DataLoadPartPrefabs();
}

void bpp::DataUnloadAll()
{
    DataUnloadMaterials();
    DataUnloadAtlases();
    DataUnloadSprites();
    DataUnloadPartPrefabs();
}
