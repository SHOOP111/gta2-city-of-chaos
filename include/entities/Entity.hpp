#pragma once

#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;

    [[nodiscard]] EntityID getID() const noexcept { return m_id; }
    void setID(EntityID id) noexcept { m_id = id; }

    [[nodiscard]] const Vec2& getPosition() const noexcept { return m_position; }
    void setPosition(const Vec2& pos) noexcept { m_position = pos; }

    [[nodiscard]] const Vec2& getVelocity() const noexcept { return m_velocity; }
    void setVelocity(const Vec2& vel) noexcept { m_velocity = vel; }

    [[nodiscard]] float getRotation() const noexcept { return m_rotation; }
    void setRotation(float rot) noexcept { m_rotation = normalizeAngle(rot); }

    [[nodiscard]] float getRadius() const noexcept { return m_radius; }
    void setRadius(float r) noexcept { m_radius = r; }

    [[nodiscard]] bool isActive() const noexcept { return m_active; }
    void setActive(bool active) noexcept { m_active = active; }

    [[nodiscard]] float getHealth() const noexcept { return m_health; }
    [[nodiscard]] float getMaxHealth() const noexcept { return m_maxHealth; }
    virtual void takeDamage(float amount) {
        m_health = std::max(0.0f, m_health - amount);
        if (m_health <= 0.0f) {
            onKilled();
        }
    }

    [[nodiscard]] bool isDead() const noexcept { return m_health <= 0.0f; }

protected:
    virtual void onKilled() {}

    EntityID m_id = INVALID_ENTITY_ID;
    Vec2 m_position = {0.0f, 0.0f};
    Vec2 m_velocity = {0.0f, 0.0f};
    float m_rotation = 0.0f;
    float m_radius = 16.0f;
    float m_health = 100.0f;
    float m_maxHealth = 100.0f;
    bool m_active = true;
};

} // namespace gta2
