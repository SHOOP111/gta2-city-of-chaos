#include "systems/GangSystem.hpp"
#include <algorithm>

namespace gta2 {

GangSystem::GangSystem() {
    reset();
}

void GangSystem::reset() {
    m_gangs.clear();

    // 1. The Zaibatsu Corporation
    GangInfo z;
    z.faction = GangFaction::Zaibatsu;
    z.name = "The Zaibatsu Corporation";
    z.leaderName = "Trey Fabrik";
    z.respect = 0.0f;
    z.primaryColor = {220, 20, 20, 255};
    z.signatureCar = VehicleType::ZaibatsuZType;
    z.signatureWeapon = WeaponType::ElectroGun;
    m_gangs[GangFaction::Zaibatsu] = z;

    // 2. The Loonies
    GangInfo l;
    l.faction = GangFaction::Loonies;
    l.name = "The Loonies";
    l.leaderName = "Elmo";
    l.respect = 0.0f;
    l.primaryColor = {50, 205, 50, 255};
    l.signatureCar = VehicleType::LoonieBuggy;
    l.signatureWeapon = WeaponType::Flamethrower;
    m_gangs[GangFaction::Loonies] = l;

    // 3. The Yakuza
    GangInfo y;
    y.faction = GangFaction::Yakuza;
    y.name = "The Yakuza";
    y.leaderName = "Johnny Zoo";
    y.respect = 0.0f;
    y.primaryColor = {30, 80, 220, 255};
    y.signatureCar = VehicleType::YakuzaMiara;
    y.signatureWeapon = WeaponType::MicroSMG;
    m_gangs[GangFaction::Yakuza] = y;
}

float GangSystem::getRespect(GangFaction faction) const {
    auto it = m_gangs.find(faction);
    if (it != m_gangs.end()) {
        return it->second.respect;
    }
    return 0.0f;
}

void GangSystem::setRespect(GangFaction faction, float respect) {
    auto it = m_gangs.find(faction);
    if (it != m_gangs.end()) {
        it->second.respect = std::clamp(respect, -100.0f, 100.0f);
    }
}

void GangSystem::addRespect(GangFaction faction, float delta) {
    auto it = m_gangs.find(faction);
    if (it != m_gangs.end()) {
        it->second.respect = std::clamp(it->second.respect + delta, -100.0f, 100.0f);
    }
}

void GangSystem::onGangMemberKilled(GangFaction victimGang, GangFaction killerGang) {
    // Killing a member of Gang A reduces respect with Gang A, and boosts respect with rival Gangs!
    if (victimGang != GangFaction::None) {
        addRespect(victimGang, -5.0f);

        for (auto& [faction, info] : m_gangs) {
            if (faction != victimGang) {
                addRespect(faction, +3.0f);
            }
        }
    }
}

void GangSystem::onMissionCompleted(GangFaction employerGang, GangFaction targetGang, float rewardRespect) {
    if (employerGang != GangFaction::None) {
        addRespect(employerGang, rewardRespect);
    }
    if (targetGang != GangFaction::None && targetGang != employerGang) {
        addRespect(targetGang, -rewardRespect * 0.6f);
    }
}

bool GangSystem::isHostile(GangFaction faction) const {
    return getRespect(faction) <= -30.0f;
}

bool GangSystem::isFriendly(GangFaction faction) const {
    return getRespect(faction) >= 30.0f;
}

bool GangSystem::canAccessMissionTier(GangFaction faction, MissionTier tier) const {
    float r = getRespect(faction);
    switch (tier) {
        case MissionTier::Green:  return r >= -20.0f;
        case MissionTier::Yellow: return r >= 25.0f;
        case MissionTier::Red:    return r >= 60.0f;
    }
    return false;
}

const GangInfo& GangSystem::getGangInfo(GangFaction faction) const {
    static GangInfo defaultInfo;
    auto it = m_gangs.find(faction);
    if (it != m_gangs.end()) {
        return it->second;
    }
    return defaultInfo;
}

} // namespace gta2
