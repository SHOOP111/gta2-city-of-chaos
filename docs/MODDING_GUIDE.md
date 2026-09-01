# Modding & Custom Content Guide

GTA 2: City of Chaos is architected to allow easy extension of vehicles, weapons, maps, and mission scripts without modifying the core engine loop.

---

## 1. Adding a Custom Vehicle

To add a new vehicle archetype, add an entry to `include/entities/VehicleTypes.hpp`:

```cpp
// Example: Adding the "Hyperion GT" Supercar
inline VehicleArchetype makeHyperionGT() {
    VehicleArchetype v;
    v.type = VehicleType::SportsExotic;
    v.name = "Hyperion GT";
    v.mass = 1100.0f;
    v.maxSpeed = 550.0f;
    v.acceleration = 700.0f;
    v.brakeForce = 900.0f;
    v.handling = 0.92f;
    v.driftGrip = 0.65f;
    v.maxHealth = 250.0f;
    v.primaryColor = {255, 40, 0, 255}; // Crimson Red
    v.secondaryColor = {20, 20, 20, 255}; // Carbon Black
    v.hasSiren = false;
    v.canEquipWeapons = true;
    return v;
}
```

---

## 2. Adding a Custom Weapon

To add a custom weapon, define its properties in `include/systems/WeaponSystem.hpp`:

```cpp
// Example: Adding the Plasma Cannon
WeaponConfig plasmaConfig;
plasmaConfig.type = WeaponType::CustomPlasma;
plasmaConfig.name = "Plasma Cannon";
plasmaConfig.damage = 120.0f;
plasmaConfig.fireRate = 4.0f; // Rounds per second
plasmaConfig.projectileSpeed = 650.0f;
plasmaConfig.spreadAngle = 0.05f; // Tight beam
plasmaConfig.isExplosive = true;
plasmaConfig.explosionRadius = 45.0f;
plasmaConfig.maxAmmo = 50;
```

---

## 3. Creating Custom Payphone Missions

Missions are composed declaratively in `src/systems/MissionSystem.cpp` using the `Mission` struct:

```cpp
Mission m;
m.id = "zaibatsu_custom_01";
m.title = "Neon Sabotage";
m.faction = GangFaction::Zaibatsu;
m.tier = MissionTier::Green;
m.minRespect = -20.0f;
m.cashReward = 15000;
m.respectReward = 30.0f;

m.objectives.push_back({
    .type = ObjectiveType::StealVehicle,
    .targetType = VehicleType::LoonieBuggy,
    .targetLocation = { 1200.0f, 3400.0f },
    .description = "Steal the Loonie Demolition Buggy"
});

m.objectives.push_back({
    .type = ObjectiveType::DeliverToPoint,
    .targetLocation = { 500.0f, 600.0f },
    .description = "Deliver buggy to Zaibatsu Chop Shop"
});

missionManager.registerMission(m);
```
