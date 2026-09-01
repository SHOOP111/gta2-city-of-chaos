#pragma once

#include <cstdint>
#include <string>

namespace gta2 {

struct GameConfig {
    // Window & Display
    int windowWidth = 1280;
    int windowHeight = 720;
    std::string windowTitle = "GTA 2: City of Chaos (Modern C++20)";
    int targetFPS = 60;
    bool fullscreen = false;
    bool vsync = true;

    // Simulation Timestep
    float fixedDeltaTime = 1.0f / 60.0f;
    int maxSubSteps = 4;

    // World & Map
    int mapWidth = 256;       // Tiles
    int mapHeight = 256;      // Tiles
    float tileSize = 64.0f;   // World units (pixels per tile)

    // Camera
    float cameraSmoothSpeed = 8.0f;
    float cameraBaseZoom = 1.0f;
    float cameraSpeedZoomFactor = 0.0015f;
    float cameraMinZoom = 0.55f;
    float cameraMaxZoom = 1.35f;

    // Audio
    float masterVolume = 0.8f;
    float sfxVolume = 0.85f;
    float radioVolume = 0.7f;
    bool enableRadio = true;

    // Game Balance & Tuning
    int initialPlayerCash = 5000;
    float initialPlayerHealth = 100.0f;
    float initialPlayerArmor = 100.0f;
    int maxVehicles = 80;
    int maxPedestrians = 120;
    int maxParticles = 2000;
    int maxProjectiles = 500;

    // Gang Respect Multipliers
    float respectPerKill = 3.0f;
    float respectRivalPenalty = 5.0f;

    // Respray Shop
    int resprayCost = 5000;
    int bombInstallCost = 10000;
};

inline GameConfig& globalConfig() {
    static GameConfig cfg;
    return cfg;
}

} // namespace gta2
