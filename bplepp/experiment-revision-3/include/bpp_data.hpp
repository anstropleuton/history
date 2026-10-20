/*
 * BPLE++ Data.
 * Copyright (C) Anstro Pleuton 2025
 * Public Domain or MIT Licensed
 */

#pragma once

#include "PxPhysicsAPI.h" // IWYU pragma: keep
#include "bpp_math.hpp"
#include "raylib.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#ifndef global
#define global extern
#endif

using namespace physx;

namespace bpp {

    struct MaterialID {
        enum {
            CONTRAPTION,
            CONTRAPTION_ALIEN,
            CONTRAPTION_LOW_FRICTION,
            CONTRAPTION_WHEEL_FRICTION,
            CONTRAPTION_WOODEN_WHEEL_FRICTION
        };
    };

    global std::unordered_map<int, PxMaterial *> materials;

    void DataLoadMaterials();
    void DataUnloadMaterials();

    struct AtlasID {
        enum {
            INGAME_ATLAS,
            INGAME_ATLAS_2,
            INGAME_ATLAS_3,
            INGAME_ATLAS_4,
            MENU_ATLAS,
            MENU_ATLAS_2,
            MENU_ATLAS_3
        };
    };

    global std::unordered_map<int, Texture2D> atlases;

    void DataLoadAtlases();
    void DataUnloadAtlases();

    struct Sprite {
        int atlasID = 0;
        Rect src = {};
    };

    struct SpriteID {
        enum {
            EGG_REGULAR,
            WOODEN_FRAME_REGULAR
        };
    };

    global std::unordered_map<int, Sprite> sprites;

    void DataLoadSprites();
    void DataUnloadSprites();

    using PartGeometry = std::variant<PxBoxGeometry, PxSphereGeometry, PxCapsuleGeometry>;

    struct PartShape {
        int materialID = 0;
        PartGeometry geometry = {};
        PxTransform transform = PxTransform(PxIdentity);
        float contactOffset = 0.0f;
    };

    struct PartSprite {
        int spriteID = 0;
        Rect dest = {};
        Vec2 origin = {};
        float rotation = 0.0f;
        BppColor tint = {};

        void Draw(Vec2 offset = Vec2(0.0f, 0.0f), float rotate = 0.0f, BppColor tint = WHITE) const
        {
            const Sprite &sprite = sprites[spriteID];
            Rect dest = this->dest;
            dest.p(dest.p() + offset);
            float rotation = this->rotation + rotate;

            DrawTexturePro(atlases[sprite.atlasID], sprite.src, dest, origin, rotation, ColorTint(this->tint, tint));
        }
    };

    struct PartLayer {
        enum {
            NORMAL = 1 << 0,
            FRAME = 1 << 1,
            MAX = 1 << 2
        };
    };

    struct PartPrefab {
        struct JointConnectionType {
            enum {
                NONE,
                SOURCE,
                TARGET
            };
        };

        std::vector<PartShape> shapes = {};
        std::vector<PartSprite> sprites = {};
        float mass = 0.0f;
        int layer = PartLayer::NORMAL;
        int jointConnectionType = JointConnectionType::NONE;
        float jointConnectionStrength = 125.0f;
    };

    struct PartSkin {
        std::string skinID = "";
        PartPrefab prefab = {};
    };

    struct PartType {
        std::string typeID = "";
        std::vector<PartSkin> skins = {};
    };

    global std::vector<PartType> parts;

    inline std::pair<std::size_t, std::size_t> FindPartPrefab(const std::string &typeID, const std::string &skinID)
    {
        for (std::size_t i = 0; i < parts.size(); i++)
        {
            if (parts[i].typeID == typeID)
            {
                for (std::size_t j = 0; j < parts[i].skins.size(); j++)
                {
                    if (parts[i].skins[j].skinID == skinID)
                    {
                        return { i, j };
                    }
                }
            }
        }

        return { (std::size_t)-1, (std::size_t)-1 };
    }

    inline const PartPrefab &GetPartPrefab(const std::string &typeID, const std::string &skinID)
    {
        std::pair<std::size_t, std::size_t> index = FindPartPrefab(typeID, skinID);

        if (index.first == (std::size_t)-1)
        {
            throw std::runtime_error("Part prefab not found");
        }

        return parts[index.first].skins[index.second].prefab;
    }

    inline bool AddPartPrefab(const std::string &typeID, const std::string &skinID, const PartPrefab &prefab)
    {
        std::pair<std::size_t, std::size_t> index = FindPartPrefab(typeID, skinID);

        if (index.first != (std::size_t)-1)
        {
            return false;
        }

        PartSkin skin;
        skin.skinID = skinID;
        skin.prefab = prefab;

        for (std::size_t i = 0; i < parts.size(); i++)
        {
            if (parts[i].typeID == typeID)
            {
                parts[i].skins.push_back(skin);
                return true;
            }
        }

        PartType type;
        type.typeID = typeID;
        type.skins.push_back(skin);

        parts.push_back(type);

        return true;
    }

    void DataLoadPartPrefabs();
    void DataUnloadPartPrefabs();

    void DataLoadAll();
    void DataUnloadAll();
} // namespace bpp
