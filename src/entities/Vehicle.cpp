#include "entities/Vehicle.hpp"
#include <cmath>

namespace gta2 {

Vehicle::Vehicle(VehicleType type, const Vec2& startPos, float initialAngle)
    : m_specs(getVehicleArchetype(type))
{
    m_position = startPos;
    m_rotation = initialAngle;
    m_health = m_specs.maxHealth;
    m_maxHealth = m_specs.maxHealth;
    m_radius = std::max(m_specs.length, m_specs.width) * 0.5f;
    m_primaryColor = m_specs.primaryColor;
    m_secondaryColor = m_specs.secondaryColor;
}

void Vehicle::update(float dt) {
    if (m_isDestroyed) {
        m_forwardSpeed = 0.0f;
        m_lateralSpeed = 0.0f;
        m_angularVelocity = 0.0f;
        return;
    }

    // Engine fire damage when critical
    if (isOnFire()) {
        m_fireBurnTimer += dt;
        takeDamage(dt * 15.0f);
        if (m_health <= 0.0f) {
            detonateCarBomb();
        }
    }

    // Siren flashing timer
    if (m_sirenActive) {
        m_sirenFlashTimer += dt * 6.0f;
    }

    // Weapon cooldowns
    if (m_machineGunCooldown > 0.0f) m_machineGunCooldown -= dt;
    if (m_oilSlickCooldown > 0.0f) m_oilSlickCooldown -= dt;

    // Longitudinal Acceleration / Braking
    if (m_throttle > 0.0f) {
        // Accelerating forward
        m_forwardSpeed += m_throttle * m_specs.acceleration * dt;
        m_forwardSpeed = std::min(m_forwardSpeed, m_specs.maxSpeed);
    } else if (m_throttle < 0.0f) {
        // Braking / Reversing
        if (m_forwardSpeed > 10.0f) {
            // Braking
            m_forwardSpeed += m_throttle * m_specs.brakeForce * dt;
        } else {
            // Reversing
            m_forwardSpeed += m_throttle * (m_specs.acceleration * 0.6f) * dt;
            m_forwardSpeed = std::max(m_forwardSpeed, -m_specs.reverseMaxSpeed);
        }
    } else {
        // Rolling resistance / Drag
        float drag = 180.0f * dt;
        if (m_forwardSpeed > drag) m_forwardSpeed -= drag;
        else if (m_forwardSpeed < -drag) m_forwardSpeed += drag;
        else m_forwardSpeed = 0.0f;
    }

    // Handbrake extra braking
    if (m_handbrake && std::abs(m_forwardSpeed) > 0.0f) {
        float hbBraking = m_specs.brakeForce * 1.5f * dt;
        if (m_forwardSpeed > hbBraking) m_forwardSpeed -= hbBraking;
        else if (m_forwardSpeed < -hbBraking) m_forwardSpeed += hbBraking;
        else m_forwardSpeed = 0.0f;
    }

    // Steering & Angular Momentum
    float speedRatio = std::clamp(std::abs(m_forwardSpeed) / (m_specs.maxSpeed * 0.5f), 0.0f, 1.0f);
    float steerFactor = m_steering * m_specs.turnSpeed * (m_forwardSpeed >= 0.0f ? 1.0f : -1.0f);
    
    // Handbrake initiates drift spin
    if (m_handbrake && std::abs(m_forwardSpeed) > 100.0f) {
        steerFactor *= 1.8f;
    }

    m_angularVelocity = std::lerp(m_angularVelocity, steerFactor * speedRatio, dt * 10.0f);
    m_rotation = normalizeAngle(m_rotation + m_angularVelocity * dt);

    // Lateral tire slip & grip physics
    float currentGrip = m_handbrake ? m_specs.handbrakeGrip : m_specs.driftGrip;
    m_lateralSpeed *= std::max(0.0f, 1.0f - (dt * 12.0f * currentGrip));

    // Detect power sliding / drifting
    m_isDrifting = (std::abs(m_lateralSpeed) > 75.0f || (m_handbrake && std::abs(m_forwardSpeed) > 120.0f));

    // Compute velocity vector in world space
    Vec2 fwd = getForwardVector();
    Vec2 right = getRightVector();
    m_velocity = (fwd * m_forwardSpeed) + (right * m_lateralSpeed);

    // Integrate position
    m_position += m_velocity * dt;
}

void Vehicle::applyImpactImpulse(const Vec2& impulse, float angularImpulse, float damage) {
    m_forwardSpeed *= 0.4f;
    m_lateralSpeed += impulse.length() * 0.3f;
    m_angularVelocity += angularImpulse;
    takeDamage(damage);
}

void Vehicle::takeDamage(float amount) {
    if (m_specs.isArmored) {
        amount *= 0.4f; // 60% armor reduction
    }
    m_health = std::max(0.0f, m_health - amount);
    if (m_health <= 0.0f && !m_isDestroyed) {
        m_isDestroyed = true;
        m_primaryColor = {35, 35, 35, 255}; // Charred burnt wreckage
        m_secondaryColor = {20, 20, 20, 255};
    }
}

void Vehicle::detonateCarBomb() noexcept {
    m_health = 0.0f;
    m_isDestroyed = true;
    m_hasCarBomb = false;
}

Vec2 Vehicle::getFrontLeftWheelPos() const noexcept {
    Vec2 fwd = getForwardVector() * (m_specs.length * 0.38f);
    Vec2 right = getRightVector() * (-m_specs.width * 0.45f);
    return m_position + fwd + right;
}

Vec2 Vehicle::getFrontRightWheelPos() const noexcept {
    Vec2 fwd = getForwardVector() * (m_specs.length * 0.38f);
    Vec2 right = getRightVector() * (m_specs.width * 0.45f);
    return m_position + fwd + right;
}

Vec2 Vehicle::getRearLeftWheelPos() const noexcept {
    Vec2 fwd = getForwardVector() * (-m_specs.length * 0.38f);
    Vec2 right = getRightVector() * (-m_specs.width * 0.45f);
    return m_position + fwd + right;
}

Vec2 Vehicle::getRearRightWheelPos() const noexcept {
    Vec2 fwd = getForwardVector() * (-m_specs.length * 0.38f);
    Vec2 right = getRightVector() * (m_specs.width * 0.45f);
    return m_position + fwd + right;
}

Rect2D Vehicle::getBoundingBox() const noexcept {
    float halfW = m_specs.width * 0.5f;
    float halfH = m_specs.length * 0.5f;
    float maxDim = std::max(halfW, halfH);
    return Rect2D(m_position.x - maxDim, m_position.y - maxDim, maxDim * 2.0f, maxDim * 2.0f);
}

} // namespace gta2
