#include "core/AssetManager.hpp"
#include <iostream>

#ifndef HEADLESS_MODE
#include "raylib.h"
#endif

namespace gta2 {

#ifndef HEADLESS_MODE
static Texture2D g_vehicleTexture{};
static Texture2D g_tilesetTexture{};
static Texture2D g_characterTexture{};
static Texture2D g_titleBannerTexture{};
#endif

void AssetManager::initialize() {
#ifndef HEADLESS_MODE
    if (FileExists("assets/vehicles.jpg")) {
        g_vehicleTexture = LoadTexture("assets/vehicles.jpg");
    }
    if (FileExists("assets/tileset.jpg")) {
        g_tilesetTexture = LoadTexture("assets/tileset.jpg");
    }
    if (FileExists("assets/characters.jpg")) {
        g_characterTexture = LoadTexture("assets/characters.jpg");
    }
    if (FileExists("assets/title_banner.jpg")) {
        g_titleBannerTexture = LoadTexture("assets/title_banner.jpg");
    }
    m_loaded = true;
#endif
}

void AssetManager::shutdown() {
#ifndef HEADLESS_MODE
    if (m_loaded) {
        if (g_vehicleTexture.id > 0) UnloadTexture(g_vehicleTexture);
        if (g_tilesetTexture.id > 0) UnloadTexture(g_tilesetTexture);
        if (g_characterTexture.id > 0) UnloadTexture(g_characterTexture);
        if (g_titleBannerTexture.id > 0) UnloadTexture(g_titleBannerTexture);
        m_loaded = false;
    }
#endif
}

SpriteRect AssetManager::getVehicleSpriteRect(VehicleType type) const {
    // 1024x1024 sprite atlas layout: 2 columns, 6 rows
    // Row 0: Sports Car, Row 1: Buggy, Row 2: Police Cruiser, Row 3: SWAT Van, Row 4: Taxi, Row 5: Tank
    switch (type) {
        case VehicleType::SportsExotic:
        case VehicleType::ZaibatsuZType:
            return {40.0f, 30.0f, 360.0f, 130.0f};
        case VehicleType::LoonieBuggy:
            return {40.0f, 180.0f, 340.0f, 140.0f};
        case VehicleType::CopCruiser:
            return {40.0f, 330.0f, 380.0f, 140.0f};
        case VehicleType::SwatVan:
        case VehicleType::FbiSedan:
            return {40.0f, 480.0f, 380.0f, 160.0f};
        case VehicleType::Taxi:
            return {40.0f, 650.0f, 360.0f, 140.0f};
        case VehicleType::ArmyTank:
            return {40.0f, 800.0f, 400.0f, 160.0f};
        default:
            return {40.0f, 30.0f, 360.0f, 130.0f};
    }
}

SpriteRect AssetManager::getTileSpriteRect(TileType type) const {
    switch (type) {
        case TileType::RoadAsphalt:
        case TileType::RoadMarking:
            return {0.0f, 0.0f, 170.0f, 170.0f};
        case TileType::Sidewalk:
            return {0.0f, 375.0f, 170.0f, 170.0f};
        case TileType::BuildingRoof:
        case TileType::BuildingWall:
            return {500.0f, 375.0f, 250.0f, 250.0f};
        case TileType::Grass:
            return {0.0f, 625.0f, 170.0f, 170.0f};
        case TileType::Water:
            return {0.0f, 875.0f, 170.0f, 125.0f};
        default:
            return {0.0f, 0.0f, 170.0f, 170.0f};
    }
}

SpriteRect AssetManager::getCharacterSpriteRect(PedestrianRole role, GangFaction gang) const {
    switch (role) {
        case PedestrianRole::Civilian:
            return {20.0f, 40.0f, 80.0f, 110.0f}; // Protagonist
        case PedestrianRole::GangMember:
            if (gang == GangFaction::Zaibatsu) {
                return {20.0f, 190.0f, 80.0f, 110.0f}; // Mobster
            } else {
                return {20.0f, 360.0f, 80.0f, 110.0f}; // Punk Anarchist
            }
        case PedestrianRole::PoliceFoot:
            return {20.0f, 520.0f, 80.0f, 110.0f}; // Police Officer
        case PedestrianRole::SwatOfficer:
            return {20.0f, 680.0f, 90.0f, 120.0f}; // SWAT Soldier
        case PedestrianRole::Paramedic:
            return {20.0f, 880.0f, 80.0f, 110.0f}; // Doctor
        default:
            return {20.0f, 40.0f, 80.0f, 110.0f};
    }
}

} // namespace gta2
