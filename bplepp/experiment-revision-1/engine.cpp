#include <cmath>
#include <format>
#include <variant>

#define DEFINE_GLOBS
#include "engine.hpp"
#include "raymath.h"

void WorldCamera2D::update()
{
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    float current_camera_zoom = camera.zoom;
    actual_zoom += GetMouseWheelMove() * zoom_amount;
    float next_zoom = std::pow(2.0f, actual_zoom);

    Vector2 mouse_offset = Vector2Subtract(GetMousePosition(), camera.offset);
    float change_with_zoom = 1.0f / (current_camera_zoom == 0.0f ? 1.0f : current_camera_zoom) - 1.0f / (next_zoom == 0.0f ? 1.0f : next_zoom);
    Vector2 target = Vector2Add(camera.target, Vector2Scale(mouse_offset, change_with_zoom));

    target = Vector2Subtract(target, Vector2Scale(GetMouseDelta(), IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) / (next_zoom == 0.0f ? 1.0f : next_zoom)));
    camera.target = target;
    camera.zoom = next_zoom;
}

WorldCamera2D::operator Camera2D()
{
    return camera;
}

void InitBuildGameMode();
void CloseBuildGameMode();
void InitPlayGameMode();
void ClosePlayGameMode();

void InitRaylib()
{
    InitWindow(960, 540, "Untitled");
    SetTargetFPS(60);
}

void InitPhysX()
{
    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
    physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());

    PxSceneDesc sceneDesc { physics->getTolerancesScale() };
    sceneDesc.gravity = { 0.0f, 9.81f, 0.0f };
    sceneDesc.cpuDispatcher = dispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    scene = physics->createScene(sceneDesc);
}

void InitData()
{
    spriteSheet[(int)SpriteSheetID::INGAME_ATLAS] = LoadTexture(RESOURCES_PATH "/IngameAtlas.png");
    spriteSheet[(int)SpriteSheetID::INGAME_ATLAS_2] = LoadTexture(RESOURCES_PATH "/IngameAtlas2.png");
    spriteSheet[(int)SpriteSheetID::INGAME_ATLAS_3] = LoadTexture(RESOURCES_PATH "/IngameAtlas3.png");
    spriteSheet[(int)SpriteSheetID::INGAME_ATLAS_4] = LoadTexture(RESOURCES_PATH "/IngameAtlas4.png");
    spriteSheet[(int)SpriteSheetID::MENU_ATLAS] = LoadTexture(RESOURCES_PATH "/MenuAtlas.png");
    spriteSheet[(int)SpriteSheetID::MENU_ATLAS_2] = LoadTexture(RESOURCES_PATH "/MenuAtlas2.png");
    spriteSheet[(int)SpriteSheetID::MENU_ATLAS_3] = LoadTexture(RESOURCES_PATH "/MenuAtlas3.png");

    sprites[(int)SpriteID::WOODEN_FRAME] = Sprite { SpriteSheetID::INGAME_ATLAS, { 681.0f, 520.0f, 105.0f, 106.0f } };
    sprites[(int)SpriteID::METAL_FRAME] = Sprite { SpriteSheetID::INGAME_ATLAS, { 681.0f, 740.0f, 105.0f, 106.0f } };
    sprites[(int)SpriteID::NORMAL_WHEEL_SUPPORT] = Sprite { SpriteSheetID::INGAME_ATLAS, { 1127.0f, 844.0f, 66.0f, 78.0f } };
    sprites[(int)SpriteID::NORMAL_WHEEL_PIVOT] = Sprite { SpriteSheetID::INGAME_ATLAS, { 1127.0f, 551.0f, 63.0f, 64.0f } };
    sprites[(int)SpriteID::MOTOR_WHEEL_SUPPORT] = Sprite { SpriteSheetID::INGAME_ATLAS, { 1127.0f, 690.0f, 66.0f, 65.0f } };
    sprites[(int)SpriteID::MOTOR_WHEEL_PIVOT] = Sprite { SpriteSheetID::INGAME_ATLAS, { 808.0f, 596.0f, 88.0f, 89.0f } };
    sprites[(int)SpriteID::SODA_BOTTLE_SUPPORT] = Sprite { SpriteSheetID::INGAME_ATLAS, { 1334.0f, 350.0f, 51.0f, 64.0f } };
    sprites[(int)SpriteID::SODA_BOTTLE] = Sprite { SpriteSheetID::INGAME_ATLAS, { 1888.0f, 814.0f, 86.0f, 46.0f } };
    sprites[(int)SpriteID::PROPELLER] = Sprite { SpriteSheetID::INGAME_ATLAS, { 1197.0f, 341.0f, 35.0f, 107.0f } };
    sprites[(int)SpriteID::BIG_ENGINE] = Sprite { SpriteSheetID::INGAME_ATLAS, { 1015.0f, 1117.0f, 113.0f, 90.0f } };

    materials[(int)MaterialID::CONTRAPTION] = physics->createMaterial(0.0f, 0.0f, 0.0f);
    materials[(int)MaterialID::CONTRAPTION]->setFrictionCombineMode(PxCombineMode::eAVERAGE);
    materials[(int)MaterialID::CONTRAPTION]->setRestitutionCombineMode(PxCombineMode::eAVERAGE);
    materials[(int)MaterialID::CONTRAPTION_ALIEN] = physics->createMaterial(0.7f, 0.5f, 0.0f);
    materials[(int)MaterialID::CONTRAPTION_ALIEN]->setFrictionCombineMode(PxCombineMode::eAVERAGE);
    materials[(int)MaterialID::CONTRAPTION_ALIEN]->setRestitutionCombineMode(PxCombineMode::eAVERAGE);
    materials[(int)MaterialID::CONTRAPTION_LOW_FRICTION] = physics->createMaterial(0.0f, 0.0f, 0.0f);
    materials[(int)MaterialID::CONTRAPTION_LOW_FRICTION]->setFrictionCombineMode(PxCombineMode::eMULTIPLY);
    materials[(int)MaterialID::CONTRAPTION_LOW_FRICTION]->setRestitutionCombineMode(PxCombineMode::eAVERAGE);
    materials[(int)MaterialID::CONTRAPTION_WHEEL_FRICTION] = physics->createMaterial(0.025f, 0.0f, 0.0f);
    materials[(int)MaterialID::CONTRAPTION_WHEEL_FRICTION]->setFrictionCombineMode(PxCombineMode::eMIN);
    materials[(int)MaterialID::CONTRAPTION_WHEEL_FRICTION]->setRestitutionCombineMode(PxCombineMode::eMIN);
    materials[(int)MaterialID::CONTRAPTION_WOODEN_WHEEL_FRICTION] = physics->createMaterial(0.05f, 0.0f, 0.0f);
    materials[(int)MaterialID::CONTRAPTION_WOODEN_WHEEL_FRICTION]->setFrictionCombineMode(PxCombineMode::eMIN);
    materials[(int)MaterialID::CONTRAPTION_WOODEN_WHEEL_FRICTION]->setRestitutionCombineMode(PxCombineMode::eMIN);

    partPrefabs[(int)PrefabID::WOODEN_FRAME].shapes = { PartShapeDesc { SpriteID::WOODEN_FRAME, MaterialID::CONTRAPTION, PxBoxGeometry { 0.5f, 0.5f, 0.5f }, PxTransform { PxIdentity }, true } };
    partPrefabs[(int)PrefabID::METAL_FRAME].shapes = { PartShapeDesc { SpriteID::METAL_FRAME, MaterialID::CONTRAPTION, PxBoxGeometry { 0.5f, 0.5f, 0.5f }, PxTransform { PxIdentity }, true } };

    partPrefabs[(int)PrefabID::NORMAL_WHEEL].shapes = {
        PartShapeDesc { SpriteID::NORMAL_WHEEL_PIVOT, MaterialID::CONTRAPTION_WHEEL_FRICTION, PxSphereGeometry { 0.33f }, PxTransform { 0.5f - 0.33f, 0.5f - 0.33f + 0.005f * 2.0f, 0.5f - 0.33f }, true },
        PartShapeDesc { SpriteID::NORMAL_WHEEL_SUPPORT, MaterialID::CONTRAPTION, PxBoxGeometry { 0.2f, 0.32f, 0.5f }, PxTransform { 0.5f - 0.2f, 0.5f - 0.32f - 0.17f, 0.5f - 0.5f }, true }
    };

    partPrefabs[(int)PrefabID::MOTOR_WHEEL].shapes = {
        PartShapeDesc { SpriteID::MOTOR_WHEEL_PIVOT, MaterialID::CONTRAPTION_WHEEL_FRICTION, PxSphereGeometry { 0.45f }, PxTransform { 0.0f, -0.075f, 0.0f }, true },
        PartShapeDesc { SpriteID::MOTOR_WHEEL_SUPPORT, MaterialID::CONTRAPTION, PxBoxGeometry { 0.3f, 0.1f, 0.5f }, PxTransform { -0.05f, 0.4f, 0.0f }, true }
    };
}

void InitEngine()
{
    InitRaylib();
    InitPhysX();
    InitData();

    InitBuildGameMode();
}

void CloseRaylib()
{
    CloseWindow();
}

void ClosePhysX()
{
    scene->release();
    dispatcher->release();
    physics->release();
    foundation->release();
}

void CloseData()
{
    materials[(int)MaterialID::CONTRAPTION]->release();
    materials[(int)MaterialID::CONTRAPTION_ALIEN]->release();
    materials[(int)MaterialID::CONTRAPTION_LOW_FRICTION]->release();
    materials[(int)MaterialID::CONTRAPTION_WHEEL_FRICTION]->release();
    materials[(int)MaterialID::CONTRAPTION_WOODEN_WHEEL_FRICTION]->release();

    UnloadTexture(spriteSheet[(int)SpriteSheetID::INGAME_ATLAS]);
    UnloadTexture(spriteSheet[(int)SpriteSheetID::INGAME_ATLAS_2]);
    UnloadTexture(spriteSheet[(int)SpriteSheetID::INGAME_ATLAS_3]);
    UnloadTexture(spriteSheet[(int)SpriteSheetID::INGAME_ATLAS_4]);
    UnloadTexture(spriteSheet[(int)SpriteSheetID::MENU_ATLAS]);
    UnloadTexture(spriteSheet[(int)SpriteSheetID::MENU_ATLAS_2]);
    UnloadTexture(spriteSheet[(int)SpriteSheetID::MENU_ATLAS_3]);
}

void CloseEngine()
{
    SwitchGameMode(GameMode::BUILD);
    CloseBuildGameMode();

    CloseData();
    ClosePhysX();
    CloseRaylib();
}

void SwitchGameMode(GameMode mode)
{
    switch (mode)
    {
        case GameMode::BUILD:
            if (currentGameMode == GameMode::BUILD) break;
            ClosePlayGameMode();
            InitBuildGameMode();
            break;
        case GameMode::PLAY:
            if (currentGameMode == GameMode::PLAY) break;
            CloseBuildGameMode();
            InitPlayGameMode();
            break;
    }
}

void InitBuildGameMode()
{
}

void CloseBuildGameMode()
{
}

void InitPlayGameMode()
{
}

void ClosePlayGameMode()
{
}

void UpdateEngine()
{
    camera.update();
}

void PartPrefab::draw(PxVec2 position)
{
    for (auto &shape : shapes)
    {
        auto &texture = spriteSheet[(int)sprites[(int)shape.spriteID].sheetID];
        auto &src = sprites[(int)shape.spriteID].src;
        Rectangle dest { position.x + shape.transform.p.x, position.y + shape.transform.p.y };

        if (auto geometry = std::get_if<PxBoxGeometry>(&shape.geometry))
        {
            dest.width = geometry->halfExtents.x * 2.0f;
            dest.height = geometry->halfExtents.y * 2.0f;
            DrawRectangleLinesEx(dest, 0.01f, PURPLE);
        }
        else if (auto geometry = std::get_if<PxCapsuleGeometry>(&shape.geometry))
        {
            dest.width = geometry->radius;
            dest.height = geometry->halfHeight * 2.0f;
            DrawRectangleLinesEx(dest, 0.01f, PURPLE);
        }
        else if (auto geometry = std::get_if<PxSphereGeometry>(&shape.geometry))
        {
            dest.width = geometry->radius * 2.0f;
            dest.height = geometry->radius * 2.0f;
            DrawRectangleLinesEx(dest, 0.01f, PURPLE);
        }

        dest.x += dest.width / 2.0f - src.width / 2.0f / 100.0f;
        dest.y += dest.height / 2.0f - src.height / 2.0f / 100.0f;
        dest.width = src.width / 100.0f;
        dest.height = src.height / 100.0f;

        Vector2 origin { dest.width / 2.0f, dest.height / 2.0f };
        dest.x += origin.x;
        dest.y += origin.y;

        DrawTexturePro(texture, src, dest, origin, 0.0f, WHITE);
        DrawTextPro(GetFontDefault(), std::format("Texture: ID: {{ {}, Width: {}, Height: {} }}, Source: {{ {}, {}, {}, {} }}, Dest: {{ {}, {}, {}, {} }}", texture.id, texture.width, texture.height, src.x, src.y, src.width, src.height, dest.x, dest.y, dest.width, dest.height).c_str(), { dest.x, dest.y }, {}, 0.0f, 0.1f, 0.01f, WHITE);
    }
}

void BuildModePart::draw()
{
    partPrefabs[(int)prefabID].draw({ (float)x, (float)y });
}
