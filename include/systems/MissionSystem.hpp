#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

struct MissionObjective {
    ObjectiveType type = ObjectiveType::KillTarget;
    std::string description;
    Vec2 targetLocation = {0.0f, 0.0f};
    VehicleType targetVehicleType = VehicleType::Sedan;
    int targetCount = 1;
    int currentCount = 0;
    bool isCompleted = false;
};

struct Mission {
    std::string id;
    std::string title;
    std::string briefing;
    GangFaction faction = GangFaction::None;
    MissionTier tier = MissionTier::Green;
    float minRespect = -20.0f;
    int cashReward = 10000;
    float respectReward = 25.0f;
    float timeLimit = 0.0f; // 0 = no time limit
    float timeRemaining = 0.0f;
    std::vector<MissionObjective> objectives;
    size_t currentObjectiveIndex = 0;
    MissionStatus status = MissionStatus::Available;
};

struct KillFrenzy {
    std::string title = "KILL FRENZY!";
    WeaponType weapon = WeaponType::RocketLauncher;
    int targetKills = 15;
    int currentKills = 0;
    float duration = 120.0f;
    float timeRemaining = 120.0f;
    int rewardCash = 50000;
    bool isActive = false;
    bool isCompleted = false;
};

class MissionSystem {
public:
    MissionSystem();

    void initializeMissions();
    void update(float dt);

    bool startMission(const std::string& missionId, float currentRespect);
    void failCurrentMission(const std::string& reason);
    void completeCurrentMission();

    void startKillFrenzy(const KillFrenzy& frenzy);
    void onKillScored(WeaponType weaponUsed);
    void onVehicleDestroyed(VehicleType vehicleType);
    void onPlayerArrivedAtLocation(const Vec2& pos, float radius = 64.0f);

    [[nodiscard]] bool hasActiveMission() const noexcept { return m_activeMission != nullptr; }
    [[nodiscard]] const Mission* getActiveMission() const noexcept { return m_activeMission; }
    [[nodiscard]] const KillFrenzy& getKillFrenzy() const noexcept { return m_killFrenzy; }
    [[nodiscard]] const std::vector<Mission>& getAllMissions() const noexcept { return m_missions; }

    [[nodiscard]] const Mission* findAvailableMission(GangFaction faction, MissionTier tier, float currentRespect) const;

private:
    std::vector<Mission> m_missions;
    Mission* m_activeMission = nullptr;
    KillFrenzy m_killFrenzy;
};

} // namespace gta2
