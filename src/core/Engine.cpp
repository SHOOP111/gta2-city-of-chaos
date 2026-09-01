#include "core/Engine.hpp"
#include <algorithm>

namespace gta2 {

Engine::Engine() = default;

void Engine::initialize(bool headless) {
    m_headless = headless;
    const auto& cfg = globalConfig();

    m_map.initialize(cfg.mapWidth, cfg.mapHeight, cfg.tileSize);
    m_player = std::make_shared<Player>(Vec2(cfg.mapWidth * cfg.tileSize * 0.5f, cfg.mapHeight * cfg.tileSize * 0.5f));
    m_player->setID(1);

    m_camera.setPosition(m_player->getPosition());
    m_camera.setTarget(m_player->getPosition());

    m_trafficSystem.spawnInitialTraffic(m_map, m_vehicles, m_pedestrians, m_rng);

    if (!m_headless) {
        m_audioSystem.initialize();
    }
}

void Engine::resetGame() {
    m_vehicles.clear();
    m_pedestrians.clear();
    m_projectiles.clear();
    m_particles.clear();

    const auto& cfg = globalConfig();
    Vec2 startPos = {cfg.mapWidth * cfg.tileSize * 0.5f, cfg.mapHeight * cfg.tileSize * 0.5f};
    m_player->respawn(startPos);

    m_gangSystem.reset();
    m_wantedSystem.clearWantedLevel();
    m_trafficSystem.spawnInitialTraffic(m_map, m_vehicles, m_pedestrians, m_rng);
}

void Engine::update(float dt) {
    ++m_tickCount;

    // 1. World & Lighting
    m_map.update(dt);
    m_dayNightCycle.update(dt);

    // 2. Player Movement / Vehicle Driving
    if (m_player->isInVehicle()) {
        for (auto& v : m_vehicles) {
            if (v->getID() == m_player->getCurrentVehicleId()) {
                m_player->setPosition(v->getPosition());
                m_player->setRotation(v->getRotation());
                m_camera.setSpeed(std::abs(v->getForwardSpeed()));
                break;
            }
        }
    } else {
        m_camera.setSpeed(m_player->getVelocity().length());
    }

    // Camera follow
    m_camera.setTarget(m_player->getPosition());
    m_camera.update(dt, m_rng);

    // 3. Entity Updates
    m_player->update(dt);

    for (auto& v : m_vehicles) {
        if (v->isActive()) v->update(dt);
    }
    for (auto& p : m_pedestrians) {
        if (p->isActive()) p->update(dt);
    }
    for (auto& proj : m_projectiles) {
        if (proj->isActive()) proj->update(dt);
    }

    // 4. Physics & Collisions
    m_physicsSystem.update(dt, m_map, m_vehicles, m_pedestrians, m_projectiles, m_particles, m_rng);

    // 5. Particles
    m_particles.update(dt);

    // 6. Systems: Traffic, Missions, Wanted, Radio, Audio
    m_trafficSystem.update(dt, m_map, m_player->getPosition(), m_vehicles, m_pedestrians, m_rng);
    m_missionSystem.update(dt);
    m_wantedSystem.update(dt, m_player->getPosition(), true);
    m_radioSystem.update(dt);
    m_audioSystem.update(dt);

    // 7. Police Threat Dispatcher
    updatePoliceDispatch(dt);

    // 8. World Triggers (Payphones, Respray shops)
    updatePayphoneInteractions();
    updateResprayGarages();

    // 9. HUD update
    m_hud.update(*m_player, m_gangSystem, m_wantedSystem, m_missionSystem, m_radioSystem, dt);

    // 10. Clean up dead entities
    cleanupDeadEntities();
}

void Engine::handlePlayerInput(
    const Vec2& moveInput,
    bool attack,
    bool actionEnterExit,
    bool handbrake,
    bool specialAction,
    bool nextWeapon,
    bool prevWeapon,
    bool nextRadio,
    float dt
) {
    if (m_player->isDead()) return;

    if (nextWeapon) m_player->selectNextWeapon();
    if (prevWeapon) m_player->selectPreviousWeapon();
    if (nextRadio) m_radioSystem.nextStation();

    // Enter / Exit Vehicle
    if (actionEnterExit) {
        if (m_player->isInVehicle()) {
            // Exit vehicle to the left
            for (auto& v : m_vehicles) {
                if (v->getID() == m_player->getCurrentVehicleId()) {
                    v->setDriverId(INVALID_ENTITY_ID);
                    m_player->setCurrentVehicleId(INVALID_ENTITY_ID);
                    m_player->setPosition(v->getPosition() + v->getRightVector() * (-v->getRadius() - 15.0f));
                    break;
                }
            }
        } else {
            // Check nearby vehicle to enter/jack
            for (auto& v : m_vehicles) {
                if (!v->isDestroyed() && v->getPosition().distanceSqTo(m_player->getPosition()) < 70.0f * 70.0f) {
                    v->setDriverId(m_player->getID());
                    m_player->setCurrentVehicleId(v->getID());
                    m_player->addCarStolen();
                    m_hud.getFloatingText().addMessage("CAR JACKED!", m_player->getPosition(), ColorRGBA::Yellow());
                    break;
                }
            }
        }
    }

    if (m_player->isInVehicle()) {
        // Vehicle Driving Controls
        for (auto& v : m_vehicles) {
            if (v->getID() == m_player->getCurrentVehicleId()) {
                v->setThrottle(moveInput.y);
                v->setSteering(moveInput.x);
                v->setHandbrake(handbrake);

                if (specialAction) {
                    v->toggleSiren();
                }

                // Drive-by shooting or vehicle weapons
                if (attack) {
                    Vec2 gunPos = v->getPosition() + v->getForwardVector() * (v->getRadius() + 10.0f);
                    spawnProjectile(WeaponType::MicroSMG, m_player->getID(), gunPos, v->getForwardVector(), 800.0f, 18.0f);
                }
                break;
            }
        }
    } else {
        // On Foot Controls
        m_player->setMoveDirection(moveInput);
        if (moveInput.lengthSq() > 0.01f) {
            m_player->setRotation(moveInput.angle());
        }

        // Weapon Attack
        if (attack) {
            updatePlayerWeaponFiring(dt);
        }
    }
}

void Engine::updatePlayerWeaponFiring(float dt) {
    WeaponType weapon = m_player->getEquippedWeapon();
    const auto& stats = m_weaponSystem.getStats(weapon);

    if (m_player->getAttackCooldown() <= 0.0f) {
        int currentAmmo = m_player->getWeaponAmmo(weapon);
        if (weapon != WeaponType::Fists && currentAmmo <= 0) {
            return;
        }

        m_player->setAttackCooldown(1.0f / stats.fireRate);
        if (weapon != WeaponType::Fists) {
            m_player->addAmmo(weapon, -1);
        }

        Vec2 fwd = Vec2::fromAngle(m_player->getRotation());
        Vec2 muzzlePos = m_player->getPosition() + fwd * (m_player->getRadius() + 6.0f);

        for (int i = 0; i < stats.projectilesPerShot; ++i) {
            float spread = (stats.spreadAngle > 0.0f) ? m_rng.nextFloat(-stats.spreadAngle, stats.spreadAngle) : 0.0f;
            Vec2 shotDir = fwd.rotated(spread);
            spawnProjectile(weapon, m_player->getID(), muzzlePos, shotDir, stats.bulletSpeed, stats.damage, stats.range);
        }

        // Sound effect & camera kick
        m_audioSystem.playSFX(SoundEffectType::PistolShot);
        m_camera.addTrauma(0.08f);

        // Pedestrian reaction to gunshot noise
        for (auto& ped : m_pedestrians) {
            if (ped->isActive() && !ped->isDead()) {
                ped->getAI().onGunshotHeard(*ped, muzzlePos);
            }
        }
    }
}

void Engine::updatePayphoneInteractions() {
    for (auto& phone : m_map.getPayphones()) {
        if (phone.isRinging && !m_missionSystem.hasActiveMission()) {
            if (phone.position.distanceSqTo(m_player->getPosition()) < 64.0f * 64.0f) {
                float respect = m_gangSystem.getRespect(phone.faction);
                const auto* mission = m_missionSystem.findAvailableMission(phone.faction, phone.tier, respect);
                if (mission) {
                    m_missionSystem.startMission(mission->id, respect);
                    phone.isRinging = false;
                    m_hud.getFloatingText().addMessage("MISSION ACCEPTED!", phone.position, ColorRGBA::Green(), 1.5f, 3.0f);
                    m_audioSystem.playSFX(SoundEffectType::MissionPassed);
                } else {
                    m_hud.getFloatingText().addMessage("RESPECT TOO LOW!", phone.position, ColorRGBA::Red(), 1.2f, 2.0f);
                }
            }
        }
    }
}

void Engine::updateResprayGarages() {
    if (!m_player->isInVehicle()) return;

    for (const auto& shop : m_map.getResprayShops()) {
        if (shop.position.distanceSqTo(m_player->getPosition()) < 80.0f * 80.0f) {
            for (auto& v : m_vehicles) {
                if (v->getID() == m_player->getCurrentVehicleId()) {
                    if (m_wantedSystem.getWantedLevel() != WantedLevel::Clean || v->getHealth() < v->getMaxHealth()) {
                        if (m_player->getCash() >= globalConfig().resprayCost) {
                            m_player->spendCash(globalConfig().resprayCost);
                            v->repaint(
                                ColorRGBA(m_rng.nextInt(30, 240), m_rng.nextInt(30, 240), m_rng.nextInt(30, 240), 255),
                                ColorRGBA(20, 20, 20, 255)
                            );
                            m_wantedSystem.onResprayUsed();
                            m_hud.getFloatingText().addMessage("RESPRAYED & REPAIRED!", shop.position, ColorRGBA::Green(), 1.5f, 3.0f);
                            m_audioSystem.playSFX(SoundEffectType::RespectBonus);
                        }
                    }
                    break;
                }
            }
        }
    }
}

void Engine::updatePoliceDispatch(float dt) {
    if (m_wantedSystem.getWantedLevel() == WantedLevel::Clean) return;

    m_copSpawnTimer += dt;
    if (m_copSpawnTimer >= 4.0f) {
        m_copSpawnTimer = 0.0f;

        int activeCops = 0;
        for (const auto& p : m_pedestrians) {
            if (p->getRole() == PedestrianRole::PoliceFoot || p->getRole() == PedestrianRole::SwatOfficer) {
                activeCops++;
            }
        }

        if (activeCops < m_wantedSystem.getMaxActiveCops()) {
            float angle = m_rng.nextFloat(0.0f, TWO_PI);
            Vec2 spawnPos = m_player->getPosition() + Vec2::fromAngle(angle, 700.0f);

            if (m_wantedSystem.shouldSpawnArmyTanks()) {
                spawnVehicle(VehicleType::ArmyTank, spawnPos, angle);
            } else if (m_wantedSystem.shouldSpawnSwatVans()) {
                spawnVehicle(VehicleType::SwatVan, spawnPos, angle);
                spawnPedestrian(PedestrianRole::SwatOfficer, GangFaction::None, spawnPos, WeaponType::AssaultRifle);
            } else {
                spawnVehicle(VehicleType::CopCruiser, spawnPos, angle);
                spawnPedestrian(PedestrianRole::PoliceFoot, GangFaction::None, spawnPos, WeaponType::Pistol);
            }
        }
    }
}

void Engine::cleanupDeadEntities() {
    // Process killed pedestrians (give player cash/respect if killed by player)
    for (auto& p : m_pedestrians) {
        if (!p->isActive() && p->isDead()) {
            if (p->getGang() != GangFaction::None) {
                m_gangSystem.onGangMemberKilled(p->getGang(), GangFaction::None);
                m_player->addCash(100);
                m_hud.getFloatingText().addMessage("RESPECT +", p->getPosition(), ColorRGBA::Cyan(), 1.0f, 1.5f);
            }
            m_missionSystem.onKillScored(p->getEquippedWeapon());
            m_player->addKill();
        }
    }

    std::erase_if(m_pedestrians, [](const std::shared_ptr<Pedestrian>& p) {
        return !p->isActive() && p->isDead();
    });

    std::erase_if(m_projectiles, [](const std::shared_ptr<Projectile>& p) {
        return !p->isActive();
    });
}

std::shared_ptr<Vehicle> Engine::spawnVehicle(VehicleType type, const Vec2& pos, float angle) {
    auto v = std::make_shared<Vehicle>(type, pos, angle);
    v->setID(m_nextEntityId++);
    m_vehicles.push_back(v);
    return v;
}

std::shared_ptr<Pedestrian> Engine::spawnPedestrian(
    PedestrianRole role,
    GangFaction gang,
    const Vec2& pos,
    WeaponType weapon
) {
    auto p = std::make_shared<Pedestrian>(role, gang, pos, weapon);
    p->setID(m_nextEntityId++);
    m_pedestrians.push_back(p);
    return p;
}

void Engine::spawnProjectile(
    WeaponType weapon,
    EntityID shooterId,
    const Vec2& pos,
    const Vec2& dir,
    float speed,
    float damage,
    float range
) {
    auto proj = std::make_shared<Projectile>(weapon, shooterId, pos, dir, speed, damage, range);
    proj->setID(m_nextEntityId++);
    m_projectiles.push_back(proj);
}

} // namespace gta2
