#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "PxPhysicsAPI.h" // IWYU pragma: keep
#include "raylib.h"       // IWYU pragma: keep
#include "raymath.h"      // IWYU pragma: keep

using namespace physx;

void engine_init();
void engine_close();
void engine_iterate();
PxVec2 engine_get_mouse_pos();

struct part_t {
    PxMaterial *material {};
    PxShape *shape {};
    PxRigidDynamic *body {};

    part_t(PxVec2 position);
    ~part_t();

    void draw();
    bool check_collision(PxVec2 pos);
};

#endif // ENGINE_HPP
