#include "entities/Player.hpp"

namespace gta2 {

Player::Player(const Vec2& startPos)
    : Pedestrian(PedestrianRole::Civilian, GangFaction::None, startPos, WeaponType::Fists)
{
    // Claude Speed signature attire (Green jacket / dark pants)
    m_shirtColor = {34, 139, 34, 255};
    m_pantsColor = {25, 25, 25, 255};
    m_cash = globalConfig().initialPlayerCash;
    m_health = globalConfig().initialPlayerHealth;
    m_armor = globalConfig().initialPlayerArmor;

    // Initialize weapon inventory
    m_inventory.push_back({WeaponType::Fists, 999, true});
    m_inventory.push_back({WeaponType::Pistol, 50, true});
    m_inventory.push_back({WeaponType::DualPistols, 0, false});
    m_inventory.push_back({WeaponType::MicroSMG, 0, false});
    m_inventory.push_back({WeaponType::Shotgun, 0, false});
    m_inventory.push_back({WeaponType::AssaultRifle, 0, false});
    m_inventory.push_back({WeaponType::RocketLauncher, 0, false});
    m_inventory.push_back({WeaponType::Flamethrower, 0, false});
    m_inventory.push_back({WeaponType::ElectroGun, 0, false});
    m_inventory.push_back({WeaponType::Grenade, 0, false});
    m_inventory.push_back({WeaponType::Molotov, 0, false});

    m_selectedWeaponIndex = 1; // Default to Pistol
    setEquippedWeapon(WeaponType::Pistol);
}

void Player::update(float dt) {
    Pedestrian::update(dt);
}

void Player::unlockWeapon(WeaponType weapon, int ammo) {
    for (size_t i = 0; i < m_inventory.size(); ++i) {
        if (m_inventory[i].type == weapon) {
            m_inventory[i].unlocked = true;
            m_inventory[i].ammo += ammo;
            m_selectedWeaponIndex = i;
            setEquippedWeapon(weapon);
            return;
        }
    }
}

void Player::addAmmo(WeaponType weapon, int ammo) {
    for (auto& slot : m_inventory) {
        if (slot.type == weapon) {
            slot.unlocked = true;
            slot.ammo += ammo;
            return;
        }
    }
}

void Player::selectNextWeapon() {
    size_t start = m_selectedWeaponIndex;
    for (size_t i = 1; i <= m_inventory.size(); ++i) {
        size_t nextIdx = (start + i) % m_inventory.size();
        if (m_inventory[nextIdx].unlocked && (m_inventory[nextIdx].ammo > 0 || m_inventory[nextIdx].type == WeaponType::Fists)) {
            m_selectedWeaponIndex = nextIdx;
            setEquippedWeapon(m_inventory[nextIdx].type);
            return;
        }
    }
}

void Player::selectPreviousWeapon() {
    size_t start = m_selectedWeaponIndex;
    for (size_t i = 1; i <= m_inventory.size(); ++i) {
        size_t prevIdx = (start + m_inventory.size() - i) % m_inventory.size();
        if (m_inventory[prevIdx].unlocked && (m_inventory[prevIdx].ammo > 0 || m_inventory[prevIdx].type == WeaponType::Fists)) {
            m_selectedWeaponIndex = prevIdx;
            setEquippedWeapon(m_inventory[prevIdx].type);
            return;
        }
    }
}

int Player::getWeaponAmmo(WeaponType weapon) const {
    for (const auto& slot : m_inventory) {
        if (slot.type == weapon) {
            return slot.ammo;
        }
    }
    return 0;
}

void Player::respawn(const Vec2& pos) {
    m_position = pos;
    m_health = globalConfig().initialPlayerHealth;
    m_armor = 0.0f;
    m_active = true;
    m_state = PedestrianState::Wandering;
    m_currentVehicleId = INVALID_ENTITY_ID;
}

} // namespace gta2
