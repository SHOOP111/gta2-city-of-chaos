#pragma once

#include "Entity.hpp"
#include "ParticleSystem.hpp"

namespace gta2 {

class Projectile : public Entity {
public:
    Projectile(
        WeaponType weaponType,
        EntityID shooterId,
        const Vec2& startPos,
        const Vec2& direction,
        float speed,
        float damage,
        float maxRange = 600.0f
    ) : m_weaponType(weaponType),
        m_shooterId(shooterId),
        m_damage(damage),
        m_maxRange(maxRange)
    {
        m_position = startPos;
        m_direction = direction.normalized();
        m_velocity = m_direction * speed;
        m_rotation = m_direction.angle();
        m_radius = (weaponType == WeaponType::RocketLauncher) ? 6.0f : 3.0f;
        m_health = 1.0f;
    }

    void update(float dt) override {
        Vec2 step = m_velocity * dt;
        m_position += step;
        m_distanceTraveled += step.length();

        if (m_distanceTraveled >= m_maxRange) {
            m_active = false;
        }

        // Special behavior for grenades / rockets
        if (m_weaponType == WeaponType::RocketLauncher) {
            m_smokeTimer += dt;
        } else if (m_weaponType == WeaponType::Grenade || m_weaponType == WeaponType::Molotov) {
            m_velocity *= std::max(0.0f, 1.0f - dt * 2.0f); // Friction deceleration
            m_fuseTimer -= dt;
            if (m_fuseTimer <= 0.0f) {
                m_active = false;
                m_isExploding = true;
            }
        }
    }

    [[nodiscard]] WeaponType getWeaponType() const noexcept { return m_weaponType; }
    [[nodiscard]] EntityID getShooterId() const noexcept { return m_shooterId; }
    [[nodiscard]] float getDamage() const noexcept { return m_damage; }
    [[nodiscard]] bool isExplosive() const noexcept {
        return m_weaponType == WeaponType::RocketLauncher ||
               m_weaponType == WeaponType::Grenade ||
               m_weaponType == WeaponType::Molotov ||
               m_weaponType == WeaponType::VehicleBomb;
    }
    [[nodiscard]] float getExplosionRadius() const noexcept {
        switch (m_weaponType) {
            case WeaponType::RocketLauncher: return 75.0f;
            case WeaponType::Grenade: return 85.0f;
            case WeaponType::Molotov: return 60.0f;
            case WeaponType::VehicleBomb: return 120.0f;
            default: return 0.0f;
        }
    }
    [[nodiscard]] bool isExploding() const noexcept { return m_isExploding; }
    void triggerExplosion() noexcept { m_isExploding = true; m_active = false; }

private:
    WeaponType m_weaponType = WeaponType::Pistol;
    EntityID m_shooterId = INVALID_ENTITY_ID;
    Vec2 m_direction = {1.0f, 0.0f};
    float m_damage = 15.0f;
    float m_distanceTraveled = 0.0f;
    float m_maxRange = 600.0f;
    float m_fuseTimer = 2.5f;
    float m_smokeTimer = 0.0f;
    bool m_isExploding = false;
};

} // namespace gta2
