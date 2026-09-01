#include "entities/Pedestrian.hpp"

namespace gta2 {

Pedestrian::Pedestrian(
    PedestrianRole role,
    GangFaction gang,
    const Vec2& startPos,
    WeaponType weapon
) : m_role(role),
    m_gang(gang),
    m_equippedWeapon(weapon)
{
    m_position = startPos;
    m_health = 100.0f;
    m_maxHealth = 100.0f;
    m_radius = 12.0f;

    // Distinct clothing per gang / role
    switch (role) {
        case PedestrianRole::PoliceFoot:
            m_shirtColor = {30, 80, 180, 255};
            m_pantsColor = {20, 30, 60, 255};
            m_equippedWeapon = WeaponType::Pistol;
            break;
        case PedestrianRole::SwatOfficer:
            m_shirtColor = {30, 40, 50, 255};
            m_pantsColor = {20, 25, 30, 255};
            m_armor = 80.0f;
            m_equippedWeapon = WeaponType::AssaultRifle;
            break;
        case PedestrianRole::FbiAgent:
            m_shirtColor = {15, 15, 15, 255};
            m_pantsColor = {15, 15, 15, 255};
            m_armor = 50.0f;
            m_equippedWeapon = WeaponType::MicroSMG;
            break;
        case PedestrianRole::Paramedic:
            m_shirtColor = {240, 240, 240, 255};
            m_pantsColor = {40, 150, 40, 255};
            break;
        case PedestrianRole::Firefighter:
            m_shirtColor = {230, 180, 30, 255};
            m_pantsColor = {40, 40, 40, 255};
            break;
        case PedestrianRole::GangMember:
            if (gang == GangFaction::Zaibatsu) {
                m_shirtColor = {200, 30, 30, 255};
                m_pantsColor = {25, 25, 25, 255};
            } else if (gang == GangFaction::Loonies) {
                m_shirtColor = {50, 205, 50, 255};
                m_pantsColor = {220, 220, 30, 255};
            } else if (gang == GangFaction::Yakuza) {
                m_shirtColor = {30, 80, 220, 255};
                m_pantsColor = {230, 230, 230, 255};
            }
            break;
        default:
            m_shirtColor = {200, 180, 160, 255};
            m_pantsColor = {60, 70, 90, 255};
            break;
    }
}

void Pedestrian::update(float dt) {
    if (m_state == PedestrianState::Dead) {
        return;
    }

    if (m_attackCooldown > 0.0f) {
        m_attackCooldown -= dt;
    }

    // Burning from Flamethrower/Molotov
    if (m_isBurning) {
        m_burnTimer += dt;
        takeDamage(dt * 20.0f);
        m_walkSpeed = 170.0f; // Flailing panic
        if (m_burnTimer >= 5.0f || m_health <= 0.0f) {
            m_isBurning = false;
        }
    }

    if (!isInVehicle()) {
        m_velocity = m_moveDirection * m_walkSpeed;
        m_position += m_velocity * dt;
    }
}

void Pedestrian::takeDamage(float amount) {
    if (m_armor > 0.0f) {
        float absorbed = std::min(m_armor, amount * 0.7f);
        m_armor -= absorbed;
        amount -= absorbed;
    }

    Entity::takeDamage(amount);
}

void Pedestrian::onKilled() {
    m_state = PedestrianState::Dead;
    m_active = false;
}

} // namespace gta2
