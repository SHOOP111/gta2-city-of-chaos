#pragma once

#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

struct Tile {
    TileType type = TileType::RoadAsphalt;
    GangFaction territory = GangFaction::None;
    float friction = 1.0f;       // 1.0 = normal asphalt, 0.4 = wet/ice, 0.2 = oil
    bool isCollidable = false;   // Solid building wall or barrier
    bool isWater = false;        // Instant kill on plunge
    bool isSidewalk = false;     // Pedestrian preference
    bool isRamp = false;         // Stunt jump ramp
    float rampAngle = 0.0f;      // Launch direction
    float rampForce = 0.0f;      // Jump impulse
    uint8_t variation = 0;       // Visual texture variation
    ColorRGBA tint = ColorRGBA::White();

    [[nodiscard]] static Tile MakeRoad(GangFaction gang = GangFaction::None) {
        Tile t;
        t.type = TileType::RoadAsphalt;
        t.territory = gang;
        t.friction = 1.0f;
        t.isCollidable = false;
        t.tint = {60, 62, 65, 255};
        return t;
    }

    [[nodiscard]] static Tile MakeSidewalk(GangFaction gang = GangFaction::None) {
        Tile t;
        t.type = TileType::Sidewalk;
        t.territory = gang;
        t.friction = 0.95f;
        t.isCollidable = false;
        t.isSidewalk = true;
        t.tint = {140, 142, 145, 255};
        return t;
    }

    [[nodiscard]] static Tile MakeBuildingWall(GangFaction gang = GangFaction::None) {
        Tile t;
        t.type = TileType::BuildingWall;
        t.territory = gang;
        t.friction = 0.0f;
        t.isCollidable = true;
        t.tint = {35, 38, 45, 255};
        return t;
    }

    [[nodiscard]] static Tile MakeBuildingRoof(GangFaction gang = GangFaction::None) {
        Tile t;
        t.type = TileType::BuildingRoof;
        t.territory = gang;
        t.friction = 0.0f;
        t.isCollidable = true;
        t.tint = {25, 28, 32, 255};
        return t;
    }

    [[nodiscard]] static Tile MakeWater() {
        Tile t;
        t.type = TileType::Water;
        t.territory = GangFaction::None;
        t.friction = 0.1f;
        t.isWater = true;
        t.isCollidable = false;
        t.tint = {30, 80, 180, 255};
        return t;
    }

    [[nodiscard]] static Tile MakeStuntRamp(float angle, float force) {
        Tile t;
        t.type = TileType::StuntRamp;
        t.friction = 1.2f;
        t.isRamp = true;
        t.rampAngle = angle;
        t.rampForce = force;
        t.tint = {220, 180, 40, 255};
        return t;
    }

    [[nodiscard]] static Tile MakeResprayShopFloor() {
        Tile t;
        t.type = TileType::ResprayShopFloor;
        t.friction = 1.0f;
        t.isCollidable = false;
        t.tint = {200, 40, 40, 255};
        return t;
    }

    [[nodiscard]] static Tile MakeBombShopFloor() {
        Tile t;
        t.type = TileType::BombShopFloor;
        t.friction = 1.0f;
        t.isCollidable = false;
        t.tint = {220, 140, 20, 255};
        return t;
    }
};

} // namespace gta2
