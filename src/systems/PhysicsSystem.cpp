#include "systems/PhysicsSystem.hpp"
#include <cmath>

namespace gta2 {

void PhysicsSystem::update(
    float dt,
    CityMap& map,
    std::vector<std::shared_ptr<Vehicle>>& vehicles,
    std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
    std::vector<std::shared_ptr<Projectile>>& projectiles,
    ParticleSystem& particles,
    FastRandom& rng
) {
    // 1. Vehicle-Map and Vehicle-Vehicle Collisions
    for (size_t i = 0; i < vehicles.size(); ++i) {
        auto& vA = *vehicles[i];
        if (!vA.isActive()) continue;

        resolveVehicleMapCollisions(vA, map, particles, rng);

        // Vehicle tire particles (skid marks / smoke)
        if (vA.isDrifting()) {
            particles.emitTireSkid(vA.getRearLeftWheelPos(), vA.getVelocity(), rng);
            particles.emitTireSkid(vA.getRearRightWheelPos(), vA.getVelocity(), rng);
        }

        for (size_t j = i + 1; j < vehicles.size(); ++j) {
            auto& vB = *vehicles[j];
            if (!vB.isActive()) continue;
            resolveVehicleVehicleCollisions(vA, vB, particles, rng);
        }
    }

    // 2. Pedestrian-Map & Vehicle-Pedestrian Collisions
    for (auto& pPtr : pedestrians) {
        auto& p = *pPtr;
        if (!p.isActive() || p.isInVehicle() || p.isDead()) continue;

        resolvePedestrianMapCollisions(p, map);

        for (auto& vPtr : vehicles) {
            auto& v = *vPtr;
            if (!v.isActive() || v.isDestroyed()) continue;
            resolveVehiclePedestrianCollisions(v, p, particles, rng);
        }
    }

    // 3. Projectile Collisions & Ballistics
    for (auto& projPtr : projectiles) {
        if (!projPtr->isActive()) continue;
        resolveProjectileCollisions(*projPtr, map, vehicles, pedestrians, particles, rng);
    }
}

void PhysicsSystem::resolveVehicleMapCollisions(
    Vehicle& v,
    const CityMap& map,
    ParticleSystem& particles,
    FastRandom& rng
) {
    float tileSize = map.getTileSize();
    Vec2 pos = v.getPosition();

    // Check corners and center
    std::vector<Vec2> checkPoints = {
        v.getFrontLeftWheelPos(),
        v.getFrontRightWheelPos(),
        v.getRearLeftWheelPos(),
        v.getRearRightWheelPos(),
        pos
    };

    for (const auto& pt : checkPoints) {
        int tx = static_cast<int>(pt.x / tileSize);
        int ty = static_cast<int>(pt.y / tileSize);
        Tile t = map.getTile(tx, ty);

        if (t.isCollidable) {
            Vec2 tileCenter = {
                (static_cast<float>(tx) + 0.5f) * tileSize,
                (static_cast<float>(ty) + 0.5f) * tileSize
            };
            Vec2 normal = (pos - tileCenter).normalized();
            if (normal.lengthSq() < 0.001f) normal = {1.0f, 0.0f};

            // Push vehicle out
            v.setPosition(pos + normal * 6.0f);
            float impactDamage = std::abs(v.getForwardSpeed()) * 0.15f;
            v.applyImpactImpulse(normal * 120.0f, rng.nextFloat(-1.5f, 1.5f), impactDamage);

            // Crash sparks
            for (int k = 0; k < 6; ++k) {
                Particle sp;
                sp.position = pt;
                sp.velocity = normal.rotated(rng.nextFloat(-0.6f, 0.6f)) * rng.nextFloat(60.0f, 180.0f);
                sp.color = ColorRGBA(255, 220, 50, 255);
                sp.size = 3.0f;
                sp.life = 0.2f;
                sp.maxLife = 0.2f;
                sp.type = ParticleType::GunSpark;
                particles.emit(sp);
            }
            break;
        } else if (t.isWater) {
            // Plunged into water
            v.takeDamage(1000.0f);
            particles.emitExplosion(pos, rng);
            break;
        }
    }
}

void PhysicsSystem::resolveVehicleVehicleCollisions(
    Vehicle& a,
    Vehicle& b,
    ParticleSystem& particles,
    FastRandom& rng
) {
    Vec2 delta = b.getPosition() - a.getPosition();
    float distSq = delta.lengthSq();
    float minDist = a.getRadius() + b.getRadius();

    if (distSq < minDist * minDist && distSq > 0.0001f) {
        float dist = std::sqrt(distSq);
        Vec2 normal = delta / dist;
        float overlap = 0.5f * (minDist - dist);

        // Separate bodies
        a.setPosition(a.getPosition() - normal * overlap);
        b.setPosition(b.getPosition() + normal * overlap);

        // Relative speed impulse
        float relSpeed = (a.getVelocity() - b.getVelocity()).length();
        float damage = relSpeed * 0.2f;

        // National Guard Tank crushes smaller vehicles!
        if (a.getSpecs().canCrushCars && !b.getSpecs().isArmored) {
            b.takeDamage(500.0f);
            particles.emitExplosion(b.getPosition(), rng);
        } else if (b.getSpecs().canCrushCars && !a.getSpecs().isArmored) {
            a.takeDamage(500.0f);
            particles.emitExplosion(a.getPosition(), rng);
        } else {
            a.applyImpactImpulse(normal * (-relSpeed * 0.5f), rng.nextFloat(-1.0f, 1.0f), damage);
            b.applyImpactImpulse(normal * (relSpeed * 0.5f), rng.nextFloat(-1.0f, 1.0f), damage);
        }
    }
}

void PhysicsSystem::resolveVehiclePedestrianCollisions(
    Vehicle& v,
    Pedestrian& p,
    ParticleSystem& particles,
    FastRandom& rng
) {
    Vec2 delta = p.getPosition() - v.getPosition();
    float distSq = delta.lengthSq();
    float minDist = v.getRadius() + p.getRadius();

    if (distSq < minDist * minDist) {
        float speed = std::abs(v.getForwardSpeed());
        if (speed > 40.0f) {
            float damage = speed * 0.6f;
            p.takeDamage(damage);
            Vec2 knockback = v.getForwardVector() * (speed * 1.5f);
            p.setPosition(p.getPosition() + knockback * 0.05f);

            // Blood splatters on road
            particles.emitBloodSplatter(p.getPosition(), v.getForwardVector(), rng, 10);
        }
    }
}

void PhysicsSystem::resolvePedestrianMapCollisions(Pedestrian& p, const CityMap& map) {
    Vec2 pos = p.getPosition();
    int tx = static_cast<int>(pos.x / map.getTileSize());
    int ty = static_cast<int>(pos.y / map.getTileSize());

    Tile t = map.getTile(tx, ty);
    if (t.isCollidable) {
        Vec2 tileCenter = {
            (static_cast<float>(tx) + 0.5f) * map.getTileSize(),
            (static_cast<float>(ty) + 0.5f) * map.getTileSize()
        };
        Vec2 normal = (pos - tileCenter).normalized();
        p.setPosition(pos + normal * 4.0f);
    } else if (t.isWater) {
        p.takeDamage(500.0f); // Drowning
    }
}

void PhysicsSystem::resolveProjectileCollisions(
    Projectile& proj,
    CityMap& map,
    std::vector<std::shared_ptr<Vehicle>>& vehicles,
    std::vector<std::shared_ptr<Pedestrian>>& pedestrians,
    ParticleSystem& particles,
    FastRandom& rng
) {
    Vec2 pos = proj.getPosition();

    // Check Map Wall Collisions
    if (map.isCollidableAt(pos)) {
        if (proj.isExplosive()) {
            proj.triggerExplosion();
        } else {
            proj.setActive(false);
            // Bullet impact spark
            Particle sp;
            sp.position = pos;
            sp.velocity = rng.nextDirection() * rng.nextFloat(20.0f, 80.0f);
            sp.color = ColorRGBA(255, 230, 80, 255);
            sp.life = 0.15f;
            sp.maxLife = 0.15f;
            particles.emit(sp);
        }
    }

    // Check Vehicle Hits
    for (auto& vPtr : vehicles) {
        auto& v = *vPtr;
        if (!v.isActive() || v.isDestroyed()) continue;
        if (v.getID() == proj.getShooterId()) continue;

        if (v.getBoundingBox().contains(pos)) {
            if (proj.isExplosive()) {
                proj.triggerExplosion();
            } else {
                v.takeDamage(proj.getDamage());
                proj.setActive(false);
            }
            break;
        }
    }

    // Check Pedestrian Hits
    for (auto& pPtr : pedestrians) {
        auto& p = *pPtr;
        if (!p.isActive() || p.isInVehicle() || p.isDead()) continue;
        if (p.getID() == proj.getShooterId()) continue;

        if (p.getPosition().distanceSqTo(pos) < (p.getRadius() + proj.getRadius()) * (p.getRadius() + proj.getRadius())) {
            if (proj.isExplosive()) {
                proj.triggerExplosion();
            } else {
                p.takeDamage(proj.getDamage());
                particles.emitBloodSplatter(pos, proj.getVelocity(), rng, 6);
                p.getAI().onDamaged(p, proj.getPosition(), proj.getShooterId());
                proj.setActive(false);
            }
            break;
        }
    }

    // Resolve Area-of-Effect Explosions
    if (proj.isExploding()) {
        float radius = proj.getExplosionRadius();
        particles.emitExplosion(pos, rng);

        for (auto& vPtr : vehicles) {
            if (!vPtr->isActive() || vPtr->isDestroyed()) continue;
            float d = vPtr->getPosition().distanceTo(pos);
            if (d < radius) {
                float falloff = 1.0f - (d / radius);
                vPtr->takeDamage(proj.getDamage() * falloff * 2.5f);
                Vec2 push = (vPtr->getPosition() - pos).normalized() * (300.0f * falloff);
                vPtr->applyImpactImpulse(push, rng.nextFloat(-2.0f, 2.0f), 0.0f);
            }
        }

        for (auto& pPtr : pedestrians) {
            if (!pPtr->isActive() || pPtr->isInVehicle() || pPtr->isDead()) continue;
            float d = pPtr->getPosition().distanceTo(pos);
            if (d < radius) {
                float falloff = 1.0f - (d / radius);
                pPtr->takeDamage(proj.getDamage() * falloff * 3.0f);
                particles.emitBloodSplatter(pPtr->getPosition(), (pPtr->getPosition() - pos), rng, 12);
            }
        }
    }
}

bool PhysicsSystem::raycast(
    const CityMap& map,
    const Vec2& start,
    const Vec2& end,
    Vec2& hitPoint,
    Vec2& hitNormal
) const {
    Vec2 dir = end - start;
    float dist = dir.length();
    if (dist < 0.001f) return false;

    Vec2 step = dir.normalized() * (map.getTileSize() * 0.5f);
    int stepCount = static_cast<int>(dist / (map.getTileSize() * 0.5f));
    Vec2 current = start;

    for (int i = 0; i < stepCount; ++i) {
        current += step;
        if (map.isCollidableAt(current)) {
            hitPoint = current;
            hitNormal = -step.normalized();
            return true;
        }
    }
    return false;
}

} // namespace gta2
