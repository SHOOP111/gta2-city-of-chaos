#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Tile.hpp"
#include "RoadGraph.hpp"
#include "../core/Math.hpp"
#include "../core/Random.hpp"

namespace gta2 {

struct GangTerritoryZone {
    GangFaction faction = GangFaction::None;
    Rect2D bounds;
    std::string zoneName;
    ColorRGBA zoneColor;
};

struct PayphoneLocation {
    uint32_t id = 0;
    Vec2 position;
    GangFaction faction = GangFaction::None;
    MissionTier tier = MissionTier::Green;
    bool isRinging = false;
    float ringTimer = 0.0f;
};

struct ResprayShopLocation {
    Vec2 position;
    Vec2 doorTrigger;
    bool isOccupied = false;
};

class CityMap {
public:
    CityMap();
    ~CityMap() = default;

    void initialize(int width, int height, float tileSize);
    void generateAnywhereCity();

    [[nodiscard]] int getWidth() const noexcept { return m_width; }
    [[nodiscard]] int getHeight() const noexcept { return m_height; }
    [[nodiscard]] float getTileSize() const noexcept { return m_tileSize; }
    [[nodiscard]] float getWorldWidth() const noexcept { return static_cast<float>(m_width) * m_tileSize; }
    [[nodiscard]] float getWorldHeight() const noexcept { return static_cast<float>(m_height) * m_tileSize; }

    [[nodiscard]] Tile getTile(int x, int y) const noexcept;
    void setTile(int x, int y, const Tile& tile) noexcept;

    [[nodiscard]] Tile getTileAtWorldPos(const Vec2& worldPos) const noexcept;
    [[nodiscard]] bool isCollidableAt(const Vec2& worldPos) const noexcept;
    [[nodiscard]] bool isWaterAt(const Vec2& worldPos) const noexcept;
    [[nodiscard]] GangFaction getTerritoryAt(const Vec2& worldPos) const noexcept;

    [[nodiscard]] const RoadGraph& getRoadGraph() const noexcept { return m_roadGraph; }
    [[nodiscard]] RoadGraph& getRoadGraph() noexcept { return m_roadGraph; }

    [[nodiscard]] const std::vector<GangTerritoryZone>& getTerritories() const noexcept { return m_territories; }
    [[nodiscard]] const std::vector<PayphoneLocation>& getPayphones() const noexcept { return m_payphones; }
    [[nodiscard]] std::vector<PayphoneLocation>& getPayphones() noexcept { return m_payphones; }
    [[nodiscard]] const std::vector<ResprayShopLocation>& getResprayShops() const noexcept { return m_resprayShops; }

    [[nodiscard]] Vec2 getRandomRoadPosition(FastRandom& rng) const;
    [[nodiscard]] Vec2 getRandomSidewalkPosition(FastRandom& rng) const;

    void update(float dt);

private:
    void generateDistrict(int startX, int startY, int w, int h, GangFaction faction, const std::string& name, ColorRGBA color);
    void buildRoadGrid();
    void placeSpecialLocations();

    int m_width = 128;
    int m_height = 128;
    float m_tileSize = 64.0f;
    std::vector<Tile> m_tiles;
    RoadGraph m_roadGraph;
    std::vector<GangTerritoryZone> m_territories;
    std::vector<PayphoneLocation> m_payphones;
    std::vector<ResprayShopLocation> m_resprayShops;
    std::vector<Vec2> m_roadSpawnPoints;
    std::vector<Vec2> m_sidewalkSpawnPoints;
};

} // namespace gta2
