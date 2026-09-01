#include "world/CityMap.hpp"
#include <algorithm>

namespace gta2 {

CityMap::CityMap() = default;

void CityMap::initialize(int width, int height, float tileSize) {
    m_width = width;
    m_height = height;
    m_tileSize = tileSize;
    m_tiles.resize(static_cast<size_t>(m_width * m_height), Tile::MakeGrass());
    generateAnywhereCity();
}

void CityMap::generateAnywhereCity() {
    m_tiles.assign(static_cast<size_t>(m_width * m_height), Tile::MakeGrass());
    m_roadGraph.clear();
    m_territories.clear();
    m_payphones.clear();
    m_resprayShops.clear();
    m_roadSpawnPoints.clear();
    m_sidewalkSpawnPoints.clear();

    // Define Gang Territories across map
    // 1. Downtown Zaibatsu Sector (Top Half)
    m_territories.push_back({
        GangFaction::Zaibatsu,
        Rect2D(0.0f, 0.0f, getWorldWidth(), getWorldHeight() * 0.4f),
        "Zaibatsu Megacity Core",
        ColorRGBA(220, 30, 30, 255)
    });

    // 2. Sunnyside Loonies Slums (Bottom-Left)
    m_territories.push_back({
        GangFaction::Loonies,
        Rect2D(0.0f, getWorldHeight() * 0.4f, getWorldWidth() * 0.5f, getWorldHeight() * 0.6f),
        "Sunnyside Loonies Ward",
        ColorRGBA(50, 205, 50, 255)
    });

    // 3. J-Town Yakuza Harbor (Bottom-Right)
    m_territories.push_back({
        GangFaction::Yakuza,
        Rect2D(getWorldWidth() * 0.5f, getWorldHeight() * 0.4f, getWorldWidth() * 0.5f, getWorldHeight() * 0.6f),
        "J-Town Dragon Docks",
        ColorRGBA(30, 80, 220, 255)
    });

    buildRoadGrid();
    placeSpecialLocations();
}

void CityMap::buildRoadGrid() {
    int blockSize = 16; // 16 tiles per city block

    // Lay out road grid and buildings
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            bool isHorizontalRoad = (y % blockSize == 0 || y % blockSize == 1 || y % blockSize == 2);
            bool isVerticalRoad = (x % blockSize == 0 || x % blockSize == 1 || x % blockSize == 2);

            Vec2 worldPos = {
                (static_cast<float>(x) + 0.5f) * m_tileSize,
                (static_cast<float>(y) + 0.5f) * m_tileSize
            };
            GangFaction territory = getTerritoryAt(worldPos);

            if (isHorizontalRoad || isVerticalRoad) {
                // Road Asphalt
                setTile(x, y, Tile::MakeRoad(territory));
                m_roadSpawnPoints.push_back(worldPos);
            } else if ((y % blockSize == 3 || y % blockSize == blockSize - 1) ||
                       (x % blockSize == 3 || x % blockSize == blockSize - 1)) {
                // Sidewalk around block perimeter
                setTile(x, y, Tile::MakeSidewalk(territory));
                m_sidewalkSpawnPoints.push_back(worldPos);
            } else {
                // Interior of block: Building walls & roofs
                if (x % blockSize == 4 || x % blockSize == blockSize - 2 ||
                    y % blockSize == 4 || y % blockSize == blockSize - 2) {
                    setTile(x, y, Tile::MakeBuildingWall(territory));
                } else {
                    setTile(x, y, Tile::MakeBuildingRoof(territory));
                }
            }
        }
    }

    // Build Road Graph Nodes at intersections
    for (int by = 0; by < m_height; by += blockSize) {
        for (int bx = 0; bx < m_width; bx += blockSize) {
            Vec2 nodePos = {
                (static_cast<float>(bx) + 1.0f) * m_tileSize,
                (static_cast<float>(by) + 1.0f) * m_tileSize
            };
            m_roadGraph.addNode(nodePos, true);
            m_roadGraph.addTrafficLight(nodePos);
        }
    }

    // Interconnect neighboring road graph nodes
    int gridW = m_width / blockSize;
    int gridH = m_height / blockSize;
    for (int gy = 0; gy < gridH; ++gy) {
        for (int gx = 0; gx < gridW; ++gx) {
            uint32_t currentId = static_cast<uint32_t>(gy * gridW + gx);
            if (gx + 1 < gridW) {
                uint32_t rightId = static_cast<uint32_t>(gy * gridW + (gx + 1));
                m_roadGraph.connectNodes(currentId, rightId, true);
            }
            if (gy + 1 < gridH) {
                uint32_t downId = static_cast<uint32_t>((gy + 1) * gridW + gx);
                m_roadGraph.connectNodes(currentId, downId, true);
            }
        }
    }

    // Place Water Canal along the southern edge
    for (int x = 0; x < m_width; ++x) {
        for (int y = m_height - 6; y < m_height; ++y) {
            setTile(x, y, Tile::MakeWater());
        }
    }
}

void CityMap::placeSpecialLocations() {
    // 1. Zaibatsu Payphones
    m_payphones.push_back({1, {600.0f, 600.0f}, GangFaction::Zaibatsu, MissionTier::Green, true, 0.0f});
    m_payphones.push_back({2, {1800.0f, 600.0f}, GangFaction::Zaibatsu, MissionTier::Yellow, false, 0.0f});
    m_payphones.push_back({3, {3200.0f, 800.0f}, GangFaction::Zaibatsu, MissionTier::Red, false, 0.0f});

    // 2. Loonies Payphones
    m_payphones.push_back({4, {800.0f, 4200.0f}, GangFaction::Loonies, MissionTier::Green, true, 0.0f});
    m_payphones.push_back({5, {1600.0f, 4800.0f}, GangFaction::Loonies, MissionTier::Yellow, false, 0.0f});
    m_payphones.push_back({6, {2400.0f, 5600.0f}, GangFaction::Loonies, MissionTier::Red, false, 0.0f});

    // 3. Yakuza Payphones
    m_payphones.push_back({7, {5000.0f, 4200.0f}, GangFaction::Yakuza, MissionTier::Green, true, 0.0f});
    m_payphones.push_back({8, {5800.0f, 4800.0f}, GangFaction::Yakuza, MissionTier::Yellow, false, 0.0f});
    m_payphones.push_back({9, {6600.0f, 5600.0f}, GangFaction::Yakuza, MissionTier::Red, false, 0.0f});

    // 4. Pay 'N' Spray Respray Garages
    m_resprayShops.push_back({{1200.0f, 1200.0f}, {1200.0f, 1200.0f}, false});
    m_resprayShops.push_back({{1400.0f, 4400.0f}, {1400.0f, 4400.0f}, false});
    m_resprayShops.push_back({{5200.0f, 4400.0f}, {5200.0f, 4400.0f}, false});

    // Mark tiles for Respray shops
    for (const auto& shop : m_resprayShops) {
        int tx = static_cast<int>(shop.position.x / m_tileSize);
        int ty = static_cast<int>(shop.position.y / m_tileSize);
        if (tx >= 0 && tx < m_width && ty >= 0 && ty < m_height) {
            setTile(tx, ty, Tile::MakeResprayShopFloor());
        }
    }
}

Tile CityMap::getTile(int x, int y) const noexcept {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return Tile::MakeBuildingWall();
    }
    return m_tiles[static_cast<size_t>(y * m_width + x)];
}

void CityMap::setTile(int x, int y, const Tile& tile) noexcept {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_tiles[static_cast<size_t>(y * m_width + x)] = tile;
    }
}

Tile CityMap::getTileAtWorldPos(const Vec2& worldPos) const noexcept {
    int x = static_cast<int>(worldPos.x / m_tileSize);
    int y = static_cast<int>(worldPos.y / m_tileSize);
    return getTile(x, y);
}

bool CityMap::isCollidableAt(const Vec2& worldPos) const noexcept {
    return getTileAtWorldPos(worldPos).isCollidable;
}

bool CityMap::isWaterAt(const Vec2& worldPos) const noexcept {
    return getTileAtWorldPos(worldPos).isWater;
}

GangFaction CityMap::getTerritoryAt(const Vec2& worldPos) const noexcept {
    for (const auto& zone : m_territories) {
        if (zone.bounds.contains(worldPos)) {
            return zone.faction;
        }
    }
    return GangFaction::None;
}

Vec2 CityMap::getRandomRoadPosition(FastRandom& rng) const {
    if (m_roadSpawnPoints.empty()) {
        return {getWorldWidth() * 0.5f, getWorldHeight() * 0.5f};
    }
    int idx = rng.nextInt(0, static_cast<int>(m_roadSpawnPoints.size()) - 1);
    return m_roadSpawnPoints[static_cast<size_t>(idx)];
}

Vec2 CityMap::getRandomSidewalkPosition(FastRandom& rng) const {
    if (m_sidewalkSpawnPoints.empty()) {
        return {getWorldWidth() * 0.5f, getWorldHeight() * 0.5f};
    }
    int idx = rng.nextInt(0, static_cast<int>(m_sidewalkSpawnPoints.size()) - 1);
    return m_sidewalkSpawnPoints[static_cast<size_t>(idx)];
}

void CityMap::update(float dt) {
    m_roadGraph.update(dt);

    for (auto& phone : m_payphones) {
        if (phone.isRinging) {
            phone.ringTimer += dt;
        }
    }
}

} // namespace gta2
