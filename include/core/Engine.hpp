#pragma once

#include <vector>
#include <memory>
#include "Types.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Config.hpp"
#include "../world/CityMap.hpp"
#include "../world/Camera.hpp"
#include "../world/DayNightCycle.hpp"
#include "../entities/Player.hpp"
#include "../entities/Vehicle.hpp"
#include "../entities/Pedestrian.hpp"
#include "../entities/Projectile.hpp"
#include "../entities/ParticleSystem.hpp"
#include "../systems/PhysicsSystem.hpp"
#include "../systems/GangSystem.hpp"
#include "../systems/WantedSystem.hpp"
#include "../systems/WeaponSystem.hpp"
#include "../systems/MissionSystem.hpp"
#include "../systems/TrafficSystem.hpp"
#include "../systems/RadioSystem.hpp"
#include "../systems/AudioSystem.hpp"
#include "../ui/HUD.hpp"

namespace gta2 {

class Engine {
public:
    Engine();
    ~Engine() = default;

    void initialize(bool headless = false);
    void resetGame();
    void update(float dt);

    // Controls injection (Player input)
    void handlePlayerInput(
        const Vec2& moveInput,
        bool attack,
        bool actionEnterExit,
        bool handbrake,
        bool specialAction,
        bool nextWeapon,
        bool prevWeapon,
        bool nextRadio,
        float dt
    );

    // Entity spawners
    std::shared_ptr<Vehicle> spawnVehicle(VehicleType type, const Vec2& pos, float angle = 0.0f);
    std::shared_ptr<Pedestrian> spawnPedestrian(
        PedestrianRole role,
        GangFaction gang,
        const Vec2& pos,
        WeaponType weapon = WeaponType::Fists
    );
    void spawnProjectile(
        WeaponType weapon,
        EntityID shooterId,
        const Vec2& pos,
        const Vec2& dir,
        float speed,
        float damage,
        float range = 600.0f
    );

    // Subsystem accessors
    [[nodiscard]] CityMap& getMap() noexcept { return m_map; }
    [[nodiscard]] const CityMap& getMap() const noexcept { return m_map; }
    [[nodiscard]] Camera2D& getCamera() noexcept { return m_camera; }
    [[nodiscard]] const Camera2D& getCamera() const noexcept { return m_camera; }
    [[nodiscard]] DayNightCycle& getDayNightCycle() noexcept { return m_dayNightCycle; }
    [[nodiscard]] const DayNightCycle& getDayNightCycle() const noexcept { return m_dayNightCycle; }
    [[nodiscard]] Player& getPlayer() noexcept { return *m_player; }
    [[nodiscard]] const Player& getPlayer() const noexcept { return *m_player; }
    [[nodiscard]] ParticleSystem& getParticles() noexcept { return m_particles; }
    [[nodiscard]] const ParticleSystem& getParticles() const noexcept { return m_particles; }
    [[nodiscard]] GangSystem& getGangSystem() noexcept { return m_gangSystem; }
    [[nodiscard]] const GangSystem& getGangSystem() const noexcept { return m_gangSystem; }
    [[nodiscard]] WantedSystem& getWantedSystem() noexcept { return m_wantedSystem; }
    [[nodiscard]] const WantedSystem& getWantedSystem() const noexcept { return m_wantedSystem; }
    [[nodiscard]] MissionSystem& getMissionSystem() noexcept { return m_missionSystem; }
    [[nodiscard]] const MissionSystem& getMissionSystem() const noexcept { return m_missionSystem; }
    [[nodiscard]] RadioSystem& getRadioSystem() noexcept { return m_radioSystem; }
    [[nodiscard]] const RadioSystem& getRadioSystem() const noexcept { return m_radioSystem; }
    [[nodiscard]] AudioSystem& getAudioSystem() noexcept { return m_audioSystem; }
    [[nodiscard]] const AudioSystem& getAudioSystem() const noexcept { return m_audioSystem; }
    [[nodiscard]] HUD& getHUD() noexcept { return m_hud; }
    [[nodiscard]] const HUD& getHUD() const noexcept { return m_hud; }

    [[nodiscard]] const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const noexcept { return m_vehicles; }
    [[nodiscard]] const std::vector<std::shared_ptr<Pedestrian>>& getPedestrians() const noexcept { return m_pedestrians; }
    [[nodiscard]] const std::vector<std::shared_ptr<Projectile>>& getProjectiles() const noexcept { return m_projectiles; }

    [[nodiscard]] FastRandom& getRNG() noexcept { return m_rng; }

    // Simulation tick count (useful for headless test verification)
    [[nodiscard]] uint64_t getTickCount() const noexcept { return m_tickCount; }

private:
    void updatePlayerWeaponFiring(float dt);
    void updatePayphoneInteractions();
    void updateResprayGarages();
    void updatePoliceDispatch(float dt);
    void cleanupDeadEntities();

    CityMap m_map;
    Camera2D m_camera;
    DayNightCycle m_dayNightCycle;
    std::shared_ptr<Player> m_player;

    std::vector<std::shared_ptr<Vehicle>> m_vehicles;
    std::vector<std::shared_ptr<Pedestrian>> m_pedestrians;
    std::vector<std::shared_ptr<Projectile>> m_projectiles;
    ParticleSystem m_particles;

    PhysicsSystem m_physicsSystem;
    GangSystem m_gangSystem;
    WantedSystem m_wantedSystem;
    WeaponSystem m_weaponSystem;
    MissionSystem m_missionSystem;
    TrafficSystem m_trafficSystem;
    RadioSystem m_radioSystem;
    AudioSystem m_audioSystem;
    HUD m_hud;

    FastRandom m_rng;
    uint64_t m_tickCount = 0;
    EntityID m_nextEntityId = 1;
    float m_copSpawnTimer = 0.0f;
    bool m_headless = false;
};

} // namespace gta2
