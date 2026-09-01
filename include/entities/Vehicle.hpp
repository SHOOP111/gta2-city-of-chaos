#pragma once

#include <vector>
#include "Entity.hpp"
#include "VehicleTypes.hpp"
#include "../core/Random.hpp"

namespace gta2 {

class Vehicle : public Entity {
public:
    explicit Vehicle(VehicleType type, const Vec2& startPos, float initialAngle = 0.0f);

    void update(float dt) override;

    // Controls input (from Player or AI)
    void setThrottle(float throttle) noexcept { m_throttle = std::clamp(throttle, -1.0f, 1.0f); }
    void setSteering(float steering) noexcept { m_steering = std::clamp(steering, -1.0f, 1.0f); }
    void setHandbrake(bool handbrake) noexcept { m_handbrake = handbrake; }
    void setSiren(bool siren) noexcept { m_sirenActive = siren; }
    void toggleSiren() noexcept { if (m_specs.hasSiren) m_sirenActive = !m_sirenActive; }

    [[nodiscard]] const VehicleArchetype& getSpecs() const noexcept { return m_specs; }
    [[nodiscard]] VehicleType getType() const noexcept { return m_specs.type; }
    [[nodiscard]] float getForwardSpeed() const noexcept { return m_forwardSpeed; }
    [[nodiscard]] float getLateralSpeed() const noexcept { return m_lateralSpeed; }
    [[nodiscard]] bool isDrifting() const noexcept { return m_isDrifting; }
    [[nodiscard]] bool isSirenActive() const noexcept { return m_sirenActive; }
    [[nodiscard]] bool isOnFire() const noexcept { return m_health < m_maxHealth * 0.25f && !m_isDestroyed; }
    [[nodiscard]] bool isDestroyed() const noexcept { return m_isDestroyed; }
    [[nodiscard]] bool isArmored() const noexcept { return m_specs.isArmored; }

    // Weaponized vehicle upgrades
    [[nodiscard]] bool hasMachineGuns() const noexcept { return m_hasMachineGuns; }
    void setMachineGuns(bool val) noexcept { m_hasMachineGuns = val; }
    [[nodiscard]] bool hasOilSlick() const noexcept { return m_hasOilSlick; }
    void setOilSlick(bool val) noexcept { m_hasOilSlick = val; }
    [[nodiscard]] bool hasCarBomb() const noexcept { return m_hasCarBomb; }
    void installCarBomb() noexcept { m_hasCarBomb = true; }
    void detonateCarBomb() noexcept;

    // Passengers / Driver
    [[nodiscard]] EntityID getDriverId() const noexcept { return m_driverId; }
    void setDriverId(EntityID id) noexcept { m_driverId = id; }
    [[nodiscard]] bool hasDriver() const noexcept { return m_driverId != INVALID_ENTITY_ID; }

    // Respray & Repaint
    void repaint(const ColorRGBA& newPrimary, const ColorRGBA& newSecondary) noexcept {
        m_primaryColor = newPrimary;
        m_secondaryColor = newSecondary;
        m_health = m_maxHealth; // Repaint also repairs
    }
    [[nodiscard]] ColorRGBA getPrimaryColor() const noexcept { return m_primaryColor; }
    [[nodiscard]] ColorRGBA getSecondaryColor() const noexcept { return m_secondaryColor; }

    void applyImpactImpulse(const Vec2& impulse, float angularImpulse, float damage);
    void takeDamage(float amount) override;

    // Wheel positions in world coordinates (for skid mark & smoke particle emission)
    [[nodiscard]] Vec2 getFrontLeftWheelPos() const noexcept;
    [[nodiscard]] Vec2 getFrontRightWheelPos() const noexcept;
    [[nodiscard]] Vec2 getRearLeftWheelPos() const noexcept;
    [[nodiscard]] Vec2 getRearRightWheelPos() const noexcept;

    [[nodiscard]] Vec2 getForwardVector() const noexcept { return Vec2::fromAngle(m_rotation); }
    [[nodiscard]] Vec2 getRightVector() const noexcept { return Vec2::fromAngle(m_rotation + PI * 0.5f); }

    [[nodiscard]] Rect2D getBoundingBox() const noexcept;

private:
    VehicleArchetype m_specs;
    ColorRGBA m_primaryColor = {0, 0, 0, 255};
    ColorRGBA m_secondaryColor = {0, 0, 0, 255};

    float m_throttle = 0.0f;
    float m_steering = 0.0f;
    bool m_handbrake = false;
    bool m_sirenActive = false;
    float m_sirenFlashTimer = 0.0f;

    float m_forwardSpeed = 0.0f;
    float m_lateralSpeed = 0.0f;
    float m_angularVelocity = 0.0f;
    bool m_isDrifting = false;
    bool m_isDestroyed = false;
    float m_fireBurnTimer = 0.0f;

    EntityID m_driverId = INVALID_ENTITY_ID;

    // Upgrades
    bool m_hasMachineGuns = false;
    bool m_hasOilSlick = false;
    bool m_hasCarBomb = false;
    float m_machineGunCooldown = 0.0f;
    float m_oilSlickCooldown = 0.0f;
};

} // namespace gta2
