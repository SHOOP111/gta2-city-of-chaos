#include "systems/WeaponSystem.hpp"

namespace gta2 {

WeaponSystem::WeaponSystem() {
    // 1. Fists (Melee)
    {
        WeaponStats w;
        w.type = WeaponType::Fists;
        w.name = "Fists";
        w.damage = 12.0f;
        w.fireRate = 2.5f;
        w.range = 28.0f;
        w.isMelee = true;
        registerWeapon(w);
    }
    // 2. Pistol
    {
        WeaponStats w;
        w.type = WeaponType::Pistol;
        w.name = "Pistol 9mm";
        w.damage = 22.0f;
        w.fireRate = 3.5f;
        w.range = 450.0f;
        w.bulletSpeed = 750.0f;
        w.spreadAngle = 0.04f;
        registerWeapon(w);
    }
    // 3. Dual Pistols
    {
        WeaponStats w;
        w.type = WeaponType::DualPistols;
        w.name = "Dual Pistols";
        w.damage = 22.0f;
        w.fireRate = 6.0f;
        w.range = 450.0f;
        w.bulletSpeed = 750.0f;
        w.spreadAngle = 0.08f;
        w.projectilesPerShot = 2;
        registerWeapon(w);
    }
    // 4. Micro-SMG / Uzi
    {
        WeaponStats w;
        w.type = WeaponType::MicroSMG;
        w.name = "Micro-SMG (Uzi)";
        w.damage = 18.0f;
        w.fireRate = 12.0f;
        w.range = 400.0f;
        w.bulletSpeed = 800.0f;
        w.spreadAngle = 0.12f;
        w.isAutomatic = true;
        registerWeapon(w);
    }
    // 5. Shotgun
    {
        WeaponStats w;
        w.type = WeaponType::Shotgun;
        w.name = "Pump Shotgun";
        w.damage = 16.0f;
        w.fireRate = 1.2f;
        w.range = 320.0f;
        w.bulletSpeed = 700.0f;
        w.spreadAngle = 0.22f;
        w.projectilesPerShot = 6;
        registerWeapon(w);
    }
    // 6. Assault Rifle (M16)
    {
        WeaponStats w;
        w.type = WeaponType::AssaultRifle;
        w.name = "M16 Assault Rifle";
        w.damage = 32.0f;
        w.fireRate = 8.0f;
        w.range = 650.0f;
        w.bulletSpeed = 950.0f;
        w.spreadAngle = 0.03f;
        w.isAutomatic = true;
        registerWeapon(w);
    }
    // 7. Rocket Launcher (RPG)
    {
        WeaponStats w;
        w.type = WeaponType::RocketLauncher;
        w.name = "Rocket Launcher (RPG)";
        w.damage = 250.0f;
        w.fireRate = 0.8f;
        w.range = 800.0f;
        w.bulletSpeed = 450.0f;
        w.createsExplosion = true;
        w.explosionRadius = 80.0f;
        registerWeapon(w);
    }
    // 8. Flamethrower
    {
        WeaponStats w;
        w.type = WeaponType::Flamethrower;
        w.name = "Flamethrower";
        w.damage = 45.0f;
        w.fireRate = 15.0f;
        w.range = 260.0f;
        w.bulletSpeed = 320.0f;
        w.spreadAngle = 0.25f;
        w.createsFire = true;
        w.isAutomatic = true;
        registerWeapon(w);
    }
    // 9. Electro-Gun
    {
        WeaponStats w;
        w.type = WeaponType::ElectroGun;
        w.name = "Electro-Gun";
        w.damage = 60.0f;
        w.fireRate = 4.0f;
        w.range = 380.0f;
        w.bulletSpeed = 1200.0f;
        w.spreadAngle = 0.05f;
        registerWeapon(w);
    }
    // 10. Grenade
    {
        WeaponStats w;
        w.type = WeaponType::Grenade;
        w.name = "Hand Grenade";
        w.damage = 200.0f;
        w.fireRate = 1.0f;
        w.range = 300.0f;
        w.bulletSpeed = 280.0f;
        w.createsExplosion = true;
        w.explosionRadius = 90.0f;
        registerWeapon(w);
    }
    // 11. Molotov Cocktail
    {
        WeaponStats w;
        w.type = WeaponType::Molotov;
        w.name = "Molotov Cocktail";
        w.damage = 120.0f;
        w.fireRate = 1.0f;
        w.range = 280.0f;
        w.bulletSpeed = 260.0f;
        w.createsFire = true;
        w.createsExplosion = true;
        w.explosionRadius = 70.0f;
        registerWeapon(w);
    }
}

void WeaponSystem::registerWeapon(const WeaponStats& stats) {
    m_weapons[stats.type] = stats;
}

const WeaponStats& WeaponSystem::getStats(WeaponType type) const {
    auto it = m_weapons.find(type);
    if (it != m_weapons.end()) {
        return it->second;
    }
    static WeaponStats defaultStats;
    return defaultStats;
}

} // namespace gta2
