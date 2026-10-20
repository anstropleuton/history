#include "engine.hpp"

#include "PxRigidDynamic.h"
#include "PxScene.h"
#include "foundation/PxVec3.h"
#include "raylib.h"

PxDefaultAllocator gAllocator {};
PxDefaultErrorCallback gErrorCallback {};
PxFoundation *gFoundation {};
PxPhysics *gPhysics {};
PxDefaultCpuDispatcher *gDispatcher {};
PxScene *gScene {};

Camera2D gCamera;

void engine_init()
{
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920 / 2, 1080 / 2, "Test");
    SetTargetFPS(60);

    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, nullptr);

    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, 9.81f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    gScene = gPhysics->createScene(sceneDesc);

    gCamera.target = { 0.0f, 0.0f };
    gCamera.zoom = 25.0f;
    gCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    gCamera.rotation = 0.0f;

    PxMaterial *gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    PxRigidStatic *groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, -1, 0, 0), *gMaterial);
    gScene->addActor(*groundPlane);
    gMaterial->release();
}

void engine_close()
{
    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    gFoundation->release();

    CloseWindow();
}

void engine_iterate()
{
    gCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    gScene->simulate(1.0f / 60.0f);
    gScene->fetchResults(true);
}

PxVec2 engine_get_mouse_pos()
{
    Vector2 v = GetScreenToWorld2D(GetMousePosition(), gCamera);
    return { v.x, v.y };
}

part_t::part_t(PxVec2 position)
{
    material = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);
    shape = gPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *material);
    body = gPhysics->createRigidDynamic(PxTransform(PxVec3(position.x, position.y, 0)));
    body->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
    body->attachShape(*shape);
    PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
    gScene->addActor(*body);
}

part_t::~part_t()
{
    gScene->removeActor(*body);
    body->release();
    shape->release();
    material->release();
}

void part_t::draw()
{
    float part_angle;
    PxVec3 part_angle_axis;
    body->getGlobalPose().q.toRadiansAndUnitAxis(part_angle, part_angle_axis);
    part_angle *= part_angle_axis.dot(PxVec3(0.0f, 0.0f, 1.0f));
    Vector2 part_pos = { body->getGlobalPose().p.x, body->getGlobalPose().p.y };
    DrawRectanglePro(Rectangle { part_pos.x, part_pos.y, 1.0f, 1.0f }, { 0.5f, 0.5f }, part_angle * RAD2DEG, RED);
}

bool part_t::check_collision(PxVec2 pos)
{
    float part_angle;
    PxVec3 part_angle_axis;
    body->getGlobalPose().q.toRadiansAndUnitAxis(part_angle, part_angle_axis);
    Vector2 part_pos = { body->getGlobalPose().p.x, body->getGlobalPose().p.y };
    Vector2 point = Vector2Add(Vector2Rotate(Vector2Subtract({ pos.x, pos.y }, part_pos), part_angle), part_pos);
    return CheckCollisionPointRec(point, { part_pos.x, part_pos.y, 1.0f, 1.0f });
}
