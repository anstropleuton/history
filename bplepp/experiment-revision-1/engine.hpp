#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <array>
#include <variant>
#include <vector>

#include "PxPhysicsAPI.h" // IWYU pragma: keep
#include "raylib.h"
using namespace physx;

#ifndef DEFINE_GLOBS
#define global extern
#define g_init
#else
#define global
#define g_init \
    {          \
    }
#endif // DEFINE_GLOBS

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "res"
#endif // RESOURCES_PATH

global PxDefaultAllocator allocator g_init;
global PxDefaultErrorCallback errorCallback g_init;
global PxFoundation *foundation g_init;
global PxPhysics *physics g_init;
global PxDefaultCpuDispatcher *dispatcher g_init;
global PxScene *scene g_init;

struct WorldCamera2D {
    Camera2D camera { .zoom = 100.0f };

    float actual_zoom { 6.64385619 };
    float zoom_amount { 0.25f };

    void update();

    operator Camera2D();
};

global WorldCamera2D camera g_init;

enum class SpriteSheetID {
    NONE = -1,
    INGAME_ATLAS = 0,
    INGAME_ATLAS_2,
    INGAME_ATLAS_3,
    INGAME_ATLAS_4,
    MENU_ATLAS,
    MENU_ATLAS_2,
    MENU_ATLAS_3,
    MAX
};

global std::array<Texture, (int)SpriteSheetID::MAX> spriteSheet g_init;

struct Sprite {
    SpriteSheetID sheetID { SpriteSheetID::NONE };
    Rectangle src {};
};

enum class SpriteID {
    NONE = -1,
    WOODEN_FRAME = 0,
    METAL_FRAME,
    NORMAL_WHEEL_SUPPORT,
    NORMAL_WHEEL_PIVOT,
    MOTOR_WHEEL_SUPPORT,
    MOTOR_WHEEL_PIVOT,
    SODA_BOTTLE_SUPPORT,
    SODA_BOTTLE,
    PROPELLER,
    BIG_ENGINE,
    MAX
};

global std::array<Sprite, (int)SpriteID::MAX> sprites g_init;

enum class MaterialID {
    NONE = -1,
    CONTRAPTION = 0,
    CONTRAPTION_ALIEN,
    CONTRAPTION_LOW_FRICTION,
    CONTRAPTION_WHEEL_FRICTION,
    CONTRAPTION_WOODEN_WHEEL_FRICTION,
    MAX
};

global std::array<PxMaterial *, (int)MaterialID::MAX> materials g_init;

struct PartShapeDesc {
    SpriteID spriteID {};
    MaterialID materialID {};
    std::variant<PxSphereGeometry, PxCapsuleGeometry, PxBoxGeometry> geometry;
    PxTransform transform {};
    bool enabled {};
};

struct PartPrefab {
    std::vector<PartShapeDesc> shapes {};
    void draw(PxVec2 position);
};

enum class PrefabID {
    NONE = -1,
    WOODEN_FRAME = 0,
    METAL_FRAME,
    NORMAL_WHEEL,
    MOTOR_WHEEL,
    SODA_BOTTLE,
    PROPELLER,
    BIG_ENGINE,
    MAX
};

global std::array<PartPrefab, (int)PrefabID::MAX> partPrefabs g_init;

enum class GameMode {
    BUILD,
    PLAY
};

global GameMode currentGameMode g_init;

struct BuildModePart {
    PrefabID prefabID;
    int x;
    int y;

    void draw();
};

struct PlayModePartSprite {
    SpriteID spriteID {};
    Rectangle dest;
};

struct PlayModePartShape {
    PlayModePartSprite sprite;
    PxShape *shape;
};

struct PlayModePart {
    std::vector<PlayModePartShape> shapes;
    PxRigidDynamic *body;

    void draw();
};

global std::vector<BuildModePart> buildModeParts g_init;
global std::vector<PlayModePart> playModeParts g_init;

void InitEngine();
void CloseEngine();
void UpdateEngine();
void DrawEngine();
void SwitchGameMode(GameMode mode);

#endif // ENGINE_HPP
