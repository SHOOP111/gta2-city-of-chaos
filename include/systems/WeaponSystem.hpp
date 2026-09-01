#pragma once

#include <string>
#include <unordered_map>
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

struct WeaponStats {
    WeaponType type = WeaponType::Fists;
    std::string name = "Fists";
    float damage = 10.0f;
    float fireRate = 2.0f;          // Attacks per second
    float range = 40.0f;           // Max distance
    float bulletSpeed = 800.0f;
    float spreadAngle = 0.0f;       // In radians
    int projectilesPerShot = 1;
    int maxAmmo = 999;
    bool isAutomatic = false;
    bool isMelee = false;
    bool isSilenced = false;
    bool createsFire = false;
    bool createsExplosion = false;
    float explosionRadius = 0.0f;
    ColorRGBA muzzleFlashColor = {255, 200, 50, 255};
};

class WeaponSystem {
public:
    WeaponSystem();

    [[nodiscard]] const WeaponStats& getStats(WeaponType type) const;

private:
    void registerWeapon(const WeaponStats& stats);

    std::unordered_map<WeaponType, WeaponStats> m_weapons;
};

} // namespace gta2
