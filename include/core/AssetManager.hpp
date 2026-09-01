#pragma once

#include <string>
#include <unordered_map>
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

struct SpriteRect {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
};

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager() = default;

    void initialize();
    void shutdown();

    [[nodiscard]] bool isLoaded() const noexcept { return m_loaded; }

    // Sprite rect lookups
    [[nodiscard]] SpriteRect getVehicleSpriteRect(VehicleType type) const;
    [[nodiscard]] SpriteRect getTileSpriteRect(TileType type) const;
    [[nodiscard]] SpriteRect getCharacterSpriteRect(PedestrianRole role, GangFaction gang) const;

private:
    bool m_loaded = false;
};

inline AssetManager& globalAssets() {
    static AssetManager mgr;
    return mgr;
}

} // namespace gta2
