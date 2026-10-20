/*
 * BPLE++ Test Engine
 * Copyright (C) Anstro Pleuton 2024-2120
 * Public Domain or MIT Licensed
 */

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "geometry/PxBoxGeometry.h"
#ifdef link
#define global
#else  // link
#define global extern
#endif // link

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "res"
#endif

#include <array>
#include <variant>
#include <vector>

#include "PxPhysicsAPI.h" // IWYU pragma: keep
#include "raylib.h"       // IWYU pragma: keep

using namespace physx;

template <typename Enum>
concept EnumWithMax = requires {
    {
        Enum::MAX
    };
};

template <typename T, EnumWithMax Enum> struct erray : std::array<T, (std::size_t)Enum::MAX> { // Yes. No, shut up.
    constexpr T &operator[](Enum n) { return std::array<T, (std::size_t)Enum::MAX>::operator[](static_cast<std::size_t>(n)); }
    constexpr const T &operator[](Enum n) const { return std::array<T, (std::size_t)Enum::MAX>::operator[](static_cast<std::size_t>(n)); }

    constexpr T &at(Enum n) { return std::array<T, (std::size_t)Enum::MAX>::at(static_cast<std::size_t>(n)); }
    constexpr const T &at(Enum n) const { return std::array<T, (std::size_t)Enum::MAX>::at(static_cast<std::size_t>(n)); }
};

struct Vec2 : PxVec2 {
    using PxVec2::PxVec2;
    Vec2() = default;

    Vec2(PxVec2 v) : PxVec2 { v.x, v.y } {}
    Vec2(Vector2 v) : PxVec2 { v.x, v.y } {}

    operator Vector2() { return { x, y }; }
};

struct Vec3 : PxVec3 {
    using PxVec3::PxVec3;
    Vec3() = default;

    Vec3(PxVec3 v) : PxVec3 { v.x, v.y, v.z } {}
    Vec3(Vector3 v) : PxVec3 { v.x, v.y, v.z } {}

    operator Vector3() { return { x, y, z }; }
};

struct Vec4 : PxVec4 {
    using PxVec4::PxVec4;
    Vec4() = default;

    Vec4(PxVec4 v) : PxVec4 { v.x, v.y, v.z, v.w } {}
    Vec4(Vector4 v) : PxVec4 { v.x, v.y, v.z, v.w } {}

    operator Vector4() { return { x, y, z, w }; }
};

struct Vec2Int : PxVec2T<int> {
    using PxVec2T<int>::PxVec2T;
    Vec2Int() = default;

    Vec2Int(PxVec2T<int> v) : PxVec2T<int> { v.x, v.y } {}
    Vec2Int(Vec2 v) : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}
    Vec2Int(PxVec2 v) : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}
    Vec2Int(Vector2 v) : PxVec2T<int> { (int)std::round(v.x), (int)std::round(v.y) } {}

    operator Vector2() { return { (float)x, (float)y }; }
    operator Vec2() { return { (float)x, (float)y }; }
    operator PxVec2() { return { (float)x, (float)y }; }
};

struct Vec3Int : PxVec3T<int> {
    using PxVec3T<int>::PxVec3T;

    Vec3Int() = default;

    Vec3Int(PxVec3T<int> v) : PxVec3T<int> { v.x, v.y, v.z } {}
    Vec3Int(Vec3 v) : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}
    Vec3Int(PxVec3 v) : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}
    Vec3Int(Vector3 v) : PxVec3T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z) } {}

    operator Vector3() { return { (float)x, (float)y, (float)z }; }
    operator Vec3() { return { (float)x, (float)y, (float)z }; }
    operator PxVec3() { return { (float)x, (float)y, (float)z }; }
};

struct Vec4Int : PxVec4T<int> {
    using PxVec4T<int>::PxVec4T;

    Vec4Int() = default;

    Vec4Int(PxVec4T<int> v) : PxVec4T<int> { v.x, v.y, v.z, v.w } {}
    Vec4Int(Vec4 v) : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}
    Vec4Int(PxVec4 v) : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}
    Vec4Int(Vector4 v) : PxVec4T<int> { (int)std::round(v.x), (int)std::round(v.y), (int)std::round(v.z), (int)std::round(v.w) } {}

    operator Vector4() { return { (float)x, (float)y, (float)z, (float)w }; }
    operator Vec4() { return { (float)x, (float)y, (float)z, (float)w }; }
    operator PxVec4() { return { (float)x, (float)y, (float)z, (float)w }; }
};

struct Quat : PxQuat {
    using PxQuat::PxQuat;
    Quat() = default;

    Quat(PxQuat q) : PxQuat { q.x, q.y, q.z, q.w } {}
    Quat(Quaternion q) : PxQuat { q.x, q.y, q.z, q.w } {}

    operator Quaternion() { return { x, y, z, w }; }
};

struct Rect : Rectangle {
    using Rectangle::Rectangle;
    Rect() = default;

    Rect(Rectangle r) : Rectangle { r.x, r.y, r.width, r.height } {}
    Rect(float _x, float _y, float _width, float _height) : Rectangle { _x, _y, _width, _height } {}
    Rect(Vec2 scale) : Rectangle { 0.0f, 0.0f, scale.x, scale.y } {}
    Rect(Vec2 position, Vec2 scale) : Rectangle { position.x, position.y, scale.x, scale.y } {}

    Vec2 p() { return { x, y }; }

    void p(Vec2 position)
    {
        x = position.x;
        y = position.y;
    }

    Vec2 s() { return { width, height }; }

    void s(Vec2 scale)
    {
        width = scale.x;
        height = scale.y;
    }

    Vec2 min() { return p(); }

    Vec2 max() { return p() + s(); }
};

struct CColor : Color {
    using Color::Color;
    CColor() = default;

    CColor(Color c) : Color { c.r, c.g, c.b, c.a } {}
    CColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : Color { _r, _g, _b, _a } {}
    CColor(int _r, int _g, int _b, int _a) : Color { (unsigned char)_r, (unsigned char)_g, (unsigned char)_b, (unsigned char)_a } {} // F you narrowing conversion error
    CColor(int hex) : Color { GetColor(hex) } {}
    CColor(Vec4 normalized) : Color { ColorFromNormalized(normalized) } {}
    CColor(Vec3 hsv) : Color { ColorFromHSV(hsv.x, hsv.y, hsv.z) } {}

    int toInt() { return ColorToInt(*this); }
    Vec4 normalize() { return ColorNormalize(*this); };
    Vec3 hsv() { return ColorToHSV(*this); }

    void tint(CColor tint) { *this = ColorTint(*this, tint); }
    void brighten(float factor) { *this = ColorBrightness(*this, factor); }
    void contrast(float contrast) { *this = ColorContrast(*this, contrast); }
};

global PxDefaultAllocator allocator;
global PxDefaultErrorCallback errorCallback;
global PxFoundation *foundation;
global PxPhysics *physics;
global PxDefaultCpuDispatcher *dispatcher;
global PxScene *scene;
global PxMaterial *groundMaterial;
inline const Vec2 terrainOffset { -0.5f, 0.5f };

typedef enum {
    NOTKEY_NONE = 0x0,
    HOTKEY_SHIFT = 0x1,
    HOTKEY_CONTROL = 0x2,
    HOTKEY_ALT = 0x4,
    HOTKEY_SUPER = 0x8,
} Hotkey;

bool IsHotkeyPressed(int hotkey);
Vec2 GetPreviousMousePosition();
int SeededRandomInt(int min, int max);
int AbsMod(int a, int b);

struct WorldCamera2D {
    Camera2D camera { .zoom = 100.0f };

    float actualZoom { 6.64385619 };
    float zoomAmount { 0.25f };

    void update()
    {
        camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        auto currentCameraZoom = camera.zoom;
        actualZoom += GetMouseWheelMove() * zoomAmount * IsHotkeyPressed(0);
        auto nextZoom { std::pow(2.0f, actualZoom) };

        Vec2 mouseOffset { (Vec2)GetMousePosition() - (Vec2)camera.offset };
        auto changeWithZoom { 1.0f / (currentCameraZoom == 0.0f ? 1.0f : currentCameraZoom) - 1.0f / (nextZoom == 0.0f ? 1.0f : nextZoom) };
        Vec2 target { (Vec2)camera.target + mouseOffset * changeWithZoom };

        auto moveAmount { IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) / (nextZoom == 0.0f ? 1.0f : nextZoom) };
        target -= (Vec2)GetMouseDelta() * moveAmount;
        camera.target = target;

        camera.zoom = nextZoom;
    }

    Vec2 getWorldPosition(Vec2 position) { return GetScreenToWorld2D(position, camera); }

    Vec2 getScreenPosition(Vec2 position) { return GetWorldToScreen2D(position, camera); }

    Vec2 getWorldMousePosition() { return GetScreenToWorld2D(GetMousePosition(), camera); }

    Vec2 getWorldMouseDelta() { return GetScreenToWorld2D(GetMouseDelta(), camera); }

    Vec2 getWorldPreviousMousePosition() { return GetScreenToWorld2D(GetPreviousMousePosition(), camera); }

    operator Camera2D() { return camera; }
};

struct Terrain {
    struct GroundTree {
        Vec2Int position {};
        CColor trunkColorDay {};
        CColor leavesColorDay {};
        CColor trunkColorNight {};
        CColor leavesColorNight {};

        void draw(WorldCamera2D camera, bool dayColor)
        {
            CColor trunkColor { dayColor ? trunkColorDay : trunkColorNight };
            CColor leavesColor { dayColor ? leavesColorDay : leavesColorNight };
            Vec2 actualPosition { camera.getScreenPosition(Vec2 { position } + terrainOffset) };
            DrawRectangleRec({ actualPosition.x, actualPosition.y - camera.camera.zoom * 2.0f, camera.camera.zoom, camera.camera.zoom * 10.0f }, trunkColor);
            DrawRectangleRec({ actualPosition.x - camera.camera.zoom * 2.0f, actualPosition.y - camera.camera.zoom * 4.0f, camera.camera.zoom * 5.0f,
                               camera.camera.zoom * 2.0f },
                             leavesColor);
            DrawRectangleRec({ actualPosition.x - camera.camera.zoom * 1.0f, actualPosition.y - camera.camera.zoom * 6.0f, camera.camera.zoom * 3.0f,
                               camera.camera.zoom * 2.0f },
                             leavesColor);
        }
    };

    std::vector<GroundTree> trees {};

    struct GroundRock {
        Vec2 position {};
        Vec2 scale {};
        float rotationMultiplier {};
        PxShape *shape {};
        PxRigidStatic *body {};

        void init()
        {
            shape = physics->createShape(PxBoxGeometry { scale.x / 2.0f, scale.y / 2.0f, 0.5f }, *groundMaterial);
            body = physics->createRigidStatic(
                PxTransform { Vec3 { position.x + terrainOffset.x, position.y + terrainOffset.y, 0.0f }, Quat { rotationMultiplier * std::atan(scale.y / scale.x), Vec3 { 0.0f, 0.0f, 1.0f } } });
            body->attachShape(*shape);
            scene->addActor(*body);
            shape->release();
        }

        void close()
        {
            scene->removeActor(*body);
            body->release();
        }

        void draw(Vec2 cameraTerrainOffset, float zoom, CColor stoneColor)
        {
            Rect rec { position, scale };
            rec.p(rec.p() * zoom + cameraTerrainOffset);
            rec.s(rec.s() * zoom);
            Vec2 origin { scale / 2.0f };
            origin *= zoom;

            DrawRectanglePro(rec, origin, rotationMultiplier * std::atan(scale.y / scale.x) * RAD2DEG, stoneColor);
        }
    };

    std::vector<GroundRock> rocks {};

    void init()
    {
        Vec2Int treePositionClusterRange { -200, 200 };
        Vec2Int treePositionXRange { 50, 100 };
        Vec2Int treePositionYRange { -2, 1 };
        Vec2Int treeCountPerClusterRange { 1, 20 };
        Vec2Int treeClusterCountRange { 4, 10 };

        Vec2Int rockPositionClusterRange { -800, 800 };
        Vec2Int rockPositionXRange { 400, 800 };
        Vec2Int rockPositionYRange { 1, 5 };
        Vec2Int rockCountPerClusterRange { 1, 20 };
        Vec2Int rockClusterCountRange { 4, 10 };

        auto randomCluster { [](Vec2Int positionRange, Vec2Int positionXRange, Vec2Int positionYRange, Vec2Int countPerClusterRange) {
            std::vector<Vec2Int> values {};
            auto position { SeededRandomInt(positionRange.x, positionRange.y) };
            auto radius { SeededRandomInt(positionXRange.x, positionXRange.y) };
            auto count { SeededRandomInt(countPerClusterRange.x, countPerClusterRange.y) };
            for (auto i { 0 }; i < count; i++)
            {
                values.push_back({ SeededRandomInt(position - radius, position + radius), SeededRandomInt(positionYRange.x, positionYRange.y) });
            }
            return values;
        } };

        trees = { { { 0, -5 } } };

        auto treeCount { SeededRandomInt(treeClusterCountRange.x, treeClusterCountRange.y) };
        for (auto i { 0 }; i < treeCount; i++)
        {
            auto values { randomCluster(treePositionClusterRange, treePositionXRange, treePositionYRange, treeCountPerClusterRange) };
            for (auto value : values)
            {
                trees.push_back(GroundTree { value });
            }
        }

        for (auto i { 0 }; i < trees.size(); i++)
        {
            auto &tree { trees[i] };
            auto f { (float)i / trees.size() };

            auto trunkColorDayA { CColor { 102, 77, 51, 255 }.normalize() };
            auto trunkColorDayB { CColor { 122, 97, 71, 255 }.normalize() };
            tree.trunkColorDay = { Vec4(trunkColorDayA * (1.0f - f) + trunkColorDayB * f) };

            auto leavesColorDayA { CColor { 60, 100, 24, 255 }.normalize() };
            auto leavesColorDayB { CColor { 80, 120, 44, 255 }.normalize() };
            tree.leavesColorDay = { Vec4(leavesColorDayA * (1.0f - f) + leavesColorDayB * f) };

            auto trunkColorNightA { CColor { 51, 38, 25, 255 }.normalize() };
            auto trunkColorNightB { CColor { 61, 48, 35, 255 }.normalize() };
            tree.trunkColorNight = { Vec4(trunkColorNightA * (1.0f - f) + trunkColorNightB * f) };

            auto leavesColorNightA { CColor { 30, 50, 12, 255 }.normalize() };
            auto leavesColorNightB { CColor { 40, 60, 22, 255 }.normalize() };
            tree.leavesColorNight = { Vec4(leavesColorNightA * (1.0f - f) + leavesColorNightB * f) };
        }

        auto rockCount { SeededRandomInt(rockClusterCountRange.x, rockClusterCountRange.y) };
        for (auto i { 0 }; i < rockCount; i++)
        {
            auto values { randomCluster(rockPositionClusterRange, rockPositionXRange, rockPositionYRange, rockCountPerClusterRange) };
            for (auto value : values)
            {
                rocks.push_back(GroundRock { Vec2Int { value.x, 0 }, Vec2Int { value.y * SeededRandomInt(2, 8), value.y }, -1.0f });
            }
        }

        for (auto &rock : rocks)
        {
            rock.init();
        }
    }

    void close()
    {
        for (auto &rock : rocks)
        {
            rock.close();
        }
        rocks.clear();
        trees.clear();
    }

    void draw(WorldCamera2D camera, bool dayColor)
    {
        CColor grassColorDay { 107, 142, 35, 255 };
        CColor dirtColorDay { 101, 67, 33, 255 };
        CColor stoneColorDay { 105, 105, 105, 255 };
        CColor grassColorNight { 53, 71, 18, 255 };
        CColor dirtColorNight { 51, 34, 17, 255 };
        CColor stoneColorNight { 53, 53, 53, 255 };
        CColor grassColor { dayColor ? grassColorDay : grassColorNight };
        CColor dirtColor { dayColor ? dirtColorDay : dirtColorNight };
        CColor stoneColor { dayColor ? stoneColorDay : stoneColorNight };

        Vec2 cameraTerrainOffset { camera.getScreenPosition(terrainOffset) };

        for (auto i { 0zu }; i < trees.size() / 2zu; i++)
        {
            trees[i].draw(camera, dayColor);
        }

        for (auto rock : rocks)
        {
            rock.draw(cameraTerrainOffset, camera.camera.zoom, CColor { stoneColor.r - 20, stoneColor.g - 20, stoneColor.b - 20, (int)stoneColor.a });
        }

        for (auto i { trees.size() / 2zu }; i < trees.size(); i++)
        {
            trees[i].draw(camera, dayColor);
        }

        DrawRectangleRec({ 0.0f, cameraTerrainOffset.y, (float)GetScreenWidth(), (float)GetScreenHeight() - cameraTerrainOffset.y }, grassColor);
        DrawRectangleRec({ 0.0f, cameraTerrainOffset.y + camera.camera.zoom, (float)GetScreenWidth(),
                           (float)GetScreenHeight() - cameraTerrainOffset.y - camera.camera.zoom },
                         dirtColor);
        DrawRectangleRec({ 0.0f, cameraTerrainOffset.y + camera.camera.zoom * 4.0f, (float)GetScreenWidth(),
                           (float)GetScreenHeight() - cameraTerrainOffset.y - camera.camera.zoom * 4.0f },
                         stoneColor);
    }
} global terrain;

// Bad Piggies stuff

enum class MaterialID {
    NONE = -1,
    CONTRAPTION,
    CONTRAPTION_ALIEN,
    CONTRAPTION_LOW_FRICTION,
    CONTRAPTION_WHEEL_FRICTION,
    CONTRAPTION_WOODEN_WHEEL_FRICTION,
    MAX
};

global erray<PxMaterial *, MaterialID> materials;

void EngineLoadMaterials();
void EngineUnloadMaterials();

enum class AtlasID {
    NONE = -1,
    INGAME_ATLAS,
    INGAME_ATLAS_2,
    INGAME_ATLAS_3,
    INGAME_ATLAS_4,
    MENU_ATLAS,
    MENU_ATLAS_2,
    MENU_ATLAS_3,
    MAX
};

global erray<Texture, AtlasID> atlases;

void EngineLoadAtlases();
void EngineUnloadAtlases();

struct Sprite {
    AtlasID atlasId {};
    Rect src {};
};

enum class SpriteID {
    NONE = -1,
    EGG,
    WOODEN_FRAME,
    MAX
};

global erray<Sprite, SpriteID> sprites;

void EngineLoadSprites();
void EngineUnloadSprites();

struct PartShape {
    MaterialID materialId {};
    std::variant<PxSphereGeometry, PxCapsuleGeometry, PxBoxGeometry> geometry {};
    PxTransform transform {};
    float contactOffset {};
};

struct PartSprite {
    SpriteID spriteId {};
    Rect dest {};
    Vec2 origin {};
    float rotationOffsetDegrees {};
    CColor tint { WHITE };
};

struct PartPrefab {
    enum class JointConnectionType {
        NONE = 0,
        SOURCE,
        TARGET
    };

    std::vector<PartShape> partShapes {};
    std::vector<PartSprite> partSprites {};
    float mass {};
    bool isFrame {};
    JointConnectionType connectionType {};
};

enum class PartPrefabID {
    NONE = -1,
    EGG,
    WOODEN_FRAME,
    // METAL_FRAME,
    // CART_WHEEL,
    // NORMAL_WHEEL,
    // BELLOWS,
    // PLANE_PROPELLER,
    // COKE_BOTTLE,
    // SODA_BOTTLE,
    // ENGINE_SMALL,
    // ENGINE,
    // ENGINE_BIG,
    MAX
};

global erray<PartPrefab, PartPrefabID> partPrefabs;

void EngineLoadPrefabs();
void EngineUnloadPrefabs();

enum class GameMode {
    NONE = -1,
    BUILD_MODE,
    PLAY_MODE,
    MAX
};

global GameMode currentGameMode;

void SwitchGameMode();

Vec2Int WorldToGrid(Vec2 position);
Vec2 GridToWorld(Vec2Int position);
Vec2Int Rounded(Vec2 position);

struct BuildModePart {
    PartPrefabID prefabId {};
    Vec2Int position {};
    int gridRotation {};
    bool isFlipped {};

    float rotationDegreesFromGridRotation(int gridRotation);
    void draw(CColor tint = WHITE);
};

global std::vector<BuildModePart> buildModeParts;

// Self note: Everything PhysX related in this HAS to be pointer
struct PlayModePart {
    PartPrefabID prefabId {};
    std::vector<PxShape *> shapes {};
    PxRigidDynamic *body {};
    std::vector<PxJoint *> joints {};

    void draw(CColor tint = WHITE);
    Vec2Int getIntPosition() { return Rounded({ body->getGlobalPose().p.x, body->getGlobalPose().p.y }); }
};

global std::vector<PlayModePart> playModeParts;

inline const std::size_t npos { (std::size_t)-1 };
std::size_t FindBuildModePartAt(Vec2Int position, bool frame);
std::size_t FindPlayModePartAt(Vec2Int position, bool frame);

bool AddPartAtPos(Vec2Int position, BuildModePart part);
bool RemovePartAtPos(Vec2Int position, bool frame);

PlayModePart InitPart(BuildModePart buildPart);
void ClosePart(PlayModePart playPart);

void EngineInit();
void EngineClose();
void EngineUpdate();
void EngineDraw();

#endif // ENGINE_HPP
