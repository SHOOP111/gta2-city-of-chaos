#pragma once

#include <vector>
#include <memory>
#include "../core/Math.hpp"
#include "../world/CityMap.hpp"
#include "../entities/Vehicle.hpp"
#include "../entities/Pedestrian.hpp"
#include "../entities/Projectile.hpp"
#include "../entities/ParticleSystem.hpp"

namespace gta2 {

class PhysicsSystem {
public:
    PhysicsSystem() = default;

    void update(
        float dt,
        CityMap& map,
        std::vector<std::shared_ptr<Vehicle>>& vehicles,
        std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
        std::vector<std::shared_ptr<Projectile>>& projectiles,
        ParticleSystem& particles,
        FastRandom& rng
    );

    // Raycast against buildings and collidable tiles
    [[nodiscard]] bool raycast(
        const CityMap& map,
        const Vec2& start,
        const Vec2& end,
        Vec2& hitPoint,
        Vec2& hitNormal
    ) const;

private:
    void resolveVehicleMapCollisions(Vehicle& v, const CityMap& map, ParticleSystem& particles, FastRandom& rng);
    void resolveVehicleVehicleCollisions(Vehicle& a, Vehicle& b, ParticleSystem& particles, FastRandom& rng);
    void resolveVehiclePedestrianCollisions(Vehicle& v, Pedestrian& p, ParticleSystem& particles, FastRandom& rng);
    void resolvePedestrianMapCollisions(Pedestrian& p, const CityMap& map);
    void resolveProjectileCollisions(
        Projectile& proj,
        CityMap& map,
        std::vector<std::shared_ptr<Vehicle>>& vehicles,
        std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
        ParticleSystem& particles,
        FastRandom& rng
    );
};

} // namespace gta2
