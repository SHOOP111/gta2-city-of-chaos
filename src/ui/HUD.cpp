#include "ui/HUD.hpp"
#include <cmath>

namespace gta2 {

HUD::HUD() {
    m_minimap.setScreenPosition({110.0f, 110.0f}, 80.0f);
}

void HUD::update(
    const Player& player,
    const GangSystem& gangSystem,
    const WantedSystem& wantedSystem,
    const MissionSystem& missionSystem,
    const RadioSystem& radioSystem,
    float dt
) {
    // Smooth cash counter animation
    m_targetCash = player.getCash();
    if (m_displayedCash < m_targetCash) {
        int diff = m_targetCash - m_displayedCash;
        m_displayedCash += std::max(1, static_cast<int>(diff * dt * 10.0f));
    } else if (m_displayedCash > m_targetCash) {
        int diff = m_displayedCash - m_targetCash;
        m_displayedCash -= std::max(1, static_cast<int>(diff * dt * 10.0f));
    }

    m_displayedMultiplier = player.getMultiplier();
    m_displayedHealth = player.getHealth();
    m_displayedArmor = player.getArmor();
    m_wantedStars = wantedSystem.getStarCount();
    m_lives = player.getLives();

    m_ammo = player.getWeaponAmmo(player.getEquippedWeapon());
    switch (player.getEquippedWeapon()) {
        case WeaponType::Fists: m_weaponName = "FISTS"; break;
        case WeaponType::Pistol: m_weaponName = "PISTOL"; break;
        case WeaponType::DualPistols: m_weaponName = "DUAL PISTOLS"; break;
        case WeaponType::MicroSMG: m_weaponName = "MICRO-SMG"; break;
        case WeaponType::Shotgun: m_weaponName = "SHOTGUN"; break;
        case WeaponType::AssaultRifle: m_weaponName = "M16 RIFLE"; break;
        case WeaponType::RocketLauncher: m_weaponName = "ROCKET LAUNCHER"; break;
        case WeaponType::Flamethrower: m_weaponName = "FLAMETHROWER"; break;
        case WeaponType::ElectroGun: m_weaponName = "ELECTRO-GUN"; break;
        case WeaponType::Grenade: m_weaponName = "GRENADE"; break;
        case WeaponType::Molotov: m_weaponName = "MOLOTOV"; break;
        default: m_weaponName = "UNKNOWN"; break;
    }

    m_radioStationName = radioSystem.getCurrentStationInfo().name;

    // Respect values
    m_zaibatsuRespect = gangSystem.getRespect(GangFaction::Zaibatsu);
    m_looniesRespect = gangSystem.getRespect(GangFaction::Loonies);
    m_yakuzaRespect = gangSystem.getRespect(GangFaction::Yakuza);

    // Active Mission info
    if (missionSystem.hasActiveMission()) {
        const auto* m = missionSystem.getActiveMission();
        m_missionTitle = m->title;
        if (m->currentObjectiveIndex < m->objectives.size()) {
            m_objectiveText = m->objectives[m->currentObjectiveIndex].description;
        } else {
            m_objectiveText = "Complete!";
        }
        m_missionTimeRemaining = m->timeRemaining;
    } else {
        m_missionTitle.clear();
        m_objectiveText.clear();
        m_missionTimeRemaining = 0.0f;
    }

    m_floatingText.update(dt);
}

} // namespace gta2
