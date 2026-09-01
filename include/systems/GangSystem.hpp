#pragma once

#include <unordered_map>
#include <string>
#include "../core/Types.hpp"

namespace gta2 {

struct GangInfo {
    GangFaction faction = GangFaction::None;
    std::string name;
    std::string leaderName;
    float respect = 0.0f; // -100.0f (Hated/Hostile) to +100.0f (Allied)
    ColorRGBA primaryColor;
    VehicleType signatureCar;
    WeaponType signatureWeapon;
};

class GangSystem {
public:
    GangSystem();

    void reset();

    [[nodiscard]] float getRespect(GangFaction faction) const;
    void setRespect(GangFaction faction, float respect);
    void addRespect(GangFaction faction, float delta);

    // GTA 2 Triad dynamic: Helping Gang A hurts Gang B & C
    void onGangMemberKilled(GangFaction victimGang, GangFaction killerGang);
    void onMissionCompleted(GangFaction employerGang, GangFaction targetGang, float rewardRespect);

    [[nodiscard]] bool isHostile(GangFaction faction) const;
    [[nodiscard]] bool isFriendly(GangFaction faction) const;
    [[nodiscard]] bool canAccessMissionTier(GangFaction faction, MissionTier tier) const;

    [[nodiscard]] const GangInfo& getGangInfo(GangFaction faction) const;
    [[nodiscard]] const std::unordered_map<GangFaction, GangInfo>& getAllGangs() const noexcept { return m_gangs; }

private:
    std::unordered_map<GangFaction, GangInfo> m_gangs;
};

} // namespace gta2
