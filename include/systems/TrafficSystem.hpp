#pragma once

#include <vector>
#include <memory>
#include "../entities/Vehicle.hpp"
#include "../entities/Pedestrian.hpp"
#include "../world/CityMap.hpp"
#include "../core/Random.hpp"

namespace gta2 {

class TrafficSystem {
public:
    TrafficSystem() = default;

    void update(
        float dt,
        CityMap& map,
        const Vec2& playerPos,
        std::vector<std::shared_ptr<Vehicle>>& vehicles,
        std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
        FastRandom& rng
    );

    void spawnInitialTraffic(
        CityMap& map,
        std::vector<std::shared_ptr<Vehicle>>& vehicles,
        std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
        FastRandom& rng
    );

private:
    void updateVehicleAI(
        Vehicle& vehicle,
        const CityMap& map,
        const std::vector<std::shared_ptr<Vehicle>>& vehicles,
        float dt,
        FastRandom& rng
    );

    void manageTrafficDensity(
        CityMap& map,
        const Vec2& playerPos,
        std::vector<std::shared_ptr<Vehicle>>& vehicles,
        std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
        FastRandom& rng
    );

    float m_spawnCheckTimer = 0.0f;
};

} // namespace gta2
