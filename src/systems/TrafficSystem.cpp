#include "systems/TrafficSystem.hpp"
#include <algorithm>

namespace gta2 {

void TrafficSystem::spawnInitialTraffic(
    CityMap& map,
    std::vector<std::shared_ptr<Vehicle>>& vehicles,
    std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
    FastRandom& rng
) {
    // Spawn civilian cars on roads
    for (int i = 0; i < 25; ++i) {
        Vec2 roadPos = map.getRandomRoadPosition(rng);
        VehicleType type = static_cast<VehicleType>(rng.nextInt(0, 4));
        auto v = std::make_shared<Vehicle>(type, roadPos, rng.nextFloat(0.0f, TWO_PI));
        v->setID(static_cast<EntityID>(vehicles.size() + 100));
        vehicles.push_back(v);
    }

    // Spawn pedestrians on sidewalks
    for (int i = 0; i < 40; ++i) {
        Vec2 walkPos = map.getRandomSidewalkPosition(rng);
        GangFaction gang = map.getTerritoryAt(walkPos);
        PedestrianRole role = (gang != GangFaction::None && rng.nextBool(0.4f))
            ? PedestrianRole::GangMember
            : PedestrianRole::Civilian;

        auto p = std::make_shared<Pedestrian>(role, gang, walkPos, WeaponType::Fists);
        p->setID(static_cast<EntityID>(pedestrians.size() + 500));
        pedestrians.push_back(p);
    }
}

void TrafficSystem::update(
    float dt,
    CityMap& map,
    const Vec2& playerPos,
    std::vector<std::shared_ptr<Vehicle>>& vehicles,
    std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
    FastRandom& rng
) {
    // Update AI for traffic vehicles
    for (auto& vPtr : vehicles) {
        auto& v = *vPtr;
        if (!v.isActive() || v.isDestroyed()) continue;
        updateVehicleAI(v, map, vehicles, dt, rng);
    }

    // Update AI for pedestrians
    for (auto& pPtr : pedestrians) {
        auto& p = *pPtr;
        if (!p.isActive() || p.isDead()) continue;
        p.getAI().update(p, map, dt, rng);
    }

    // Manage dynamic spawning around player
    m_spawnCheckTimer += dt;
    if (m_spawnCheckTimer >= 1.0f) {
        m_spawnCheckTimer = 0.0f;
        manageTrafficDensity(map, playerPos, vehicles, pedestrians, rng);
    }
}

void TrafficSystem::updateVehicleAI(
    Vehicle& vehicle,
    const CityMap& map,
    const std::vector<std::shared_ptr<Vehicle>>& vehicles,
    float dt,
    FastRandom& rng
) {
    // Simple cruise control with obstacle / traffic avoidance
    Vec2 forwardPos = vehicle.getPosition() + vehicle.getForwardVector() * 80.0f;

    bool obstacleAhead = false;
    for (const auto& other : vehicles) {
        if (other->getID() == vehicle.getID()) continue;
        if (other->getPosition().distanceSqTo(forwardPos) < 50.0f * 50.0f) {
            obstacleAhead = true;
            break;
        }
    }

    if (obstacleAhead) {
        vehicle.setThrottle(-0.4f); // Brake
        vehicle.setSteering(rng.nextFloat(-0.5f, 0.5f));
    } else {
        vehicle.setThrottle(0.65f); // Normal cruising speed
        // Follow road lane
        if (map.isCollidableAt(forwardPos)) {
            vehicle.setSteering(1.0f); // Turn away from walls
        } else {
            vehicle.setSteering(0.0f);
        }
    }
}

void TrafficSystem::manageTrafficDensity(
    CityMap& map,
    const Vec2& playerPos,
    std::vector<std::shared_ptr<Vehicle>>& vehicles,
    std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
    FastRandom& rng
) {
    constexpr float despawnDistSq = 1800.0f * 1800.0f;

    // Despawn far away non-player vehicles
    std::erase_if(vehicles, [&](const std::shared_ptr<Vehicle>& v) {
        return !v->hasDriver() && v->getPosition().distanceSqTo(playerPos) > despawnDistSq;
    });

    // Despawn far away pedestrians
    std::erase_if(pedestrians, [&](const std::shared_ptr<Pedestrian>& p) {
        return p->getPosition().distanceSqTo(playerPos) > despawnDistSq;
    });

    // Replenish traffic near player if below threshold
    if (vehicles.size() < 30) {
        float angle = rng.nextFloat(0.0f, TWO_PI);
        Vec2 spawnPos = playerPos + Vec2::fromAngle(angle, rng.nextFloat(600.0f, 1000.0f));
        if (!map.isCollidableAt(spawnPos) && !map.isWaterAt(spawnPos)) {
            VehicleType type = static_cast<VehicleType>(rng.nextInt(0, 4));
            auto v = std::make_shared<Vehicle>(type, spawnPos, angle);
            v->setID(static_cast<EntityID>(vehicles.size() + 200));
            vehicles.push_back(v);
        }
    }
}

} // namespace gta2
