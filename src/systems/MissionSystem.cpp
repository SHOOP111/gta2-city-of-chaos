#include "systems/MissionSystem.hpp"
#include <algorithm>

namespace gta2 {

MissionSystem::MissionSystem() {
    initializeMissions();
}

void MissionSystem::initializeMissions() {
    m_missions.clear();

    // 1. Zaibatsu Green Mission
    {
        Mission m;
        m.id = "z_green_01";
        m.title = "Cop Shakedown";
        m.briefing = "Trey Fabrik here! We need a clean Police Squad Cruiser at the chop shop immediately. Don't scratch it!";
        m.faction = GangFaction::Zaibatsu;
        m.tier = MissionTier::Green;
        m.minRespect = -20.0f;
        m.cashReward = 10000;
        m.respectReward = 25.0f;
        m.timeLimit = 180.0f;

        MissionObjective o1;
        o1.type = ObjectiveType::StealVehicle;
        o1.targetVehicleType = VehicleType::CopCruiser;
        o1.description = "Steal a Police Cruiser";
        m.objectives.push_back(o1);

        MissionObjective o2;
        o2.type = ObjectiveType::DeliverToPoint;
        o2.targetLocation = {1200.0f, 1200.0f};
        o2.description = "Deliver to Zaibatsu Chop Shop";
        m.objectives.push_back(o2);

        m_missions.push_back(m);
    }

    // 2. Zaibatsu Yellow Mission
    {
        Mission m;
        m.id = "z_yellow_01";
        m.title = "Loonie Lab Purge";
        m.briefing = "The Loonies are concocting dangerous neuro-toxins in Sunnyside. Eliminate 5 Loonie guards!";
        m.faction = GangFaction::Zaibatsu;
        m.tier = MissionTier::Yellow;
        m.minRespect = 25.0f;
        m.cashReward = 25000;
        m.respectReward = 40.0f;
        m.timeLimit = 240.0f;

        MissionObjective o1;
        o1.type = ObjectiveType::KillTarget;
        o1.targetCount = 5;
        o1.description = "Neutralize 5 Loonie syndicate operatives";
        m.objectives.push_back(o1);

        m_missions.push_back(m);
    }

    // 3. Loonies Green Mission
    {
        Mission m;
        m.id = "l_green_01";
        m.title = "Joyride Pandemonium";
        m.briefing = "Hahaha! Elmo wants fireworks! Steal a Taxi and ram 3 Zaibatsu vehicles into junk metal!";
        m.faction = GangFaction::Loonies;
        m.tier = MissionTier::Green;
        m.minRespect = -20.0f;
        m.cashReward = 12000;
        m.respectReward = 25.0f;
        m.timeLimit = 180.0f;

        MissionObjective o1;
        o1.type = ObjectiveType::StealVehicle;
        o1.targetVehicleType = VehicleType::Taxi;
        o1.description = "Hijack a City Taxi";
        m.objectives.push_back(o1);

        MissionObjective o2;
        o2.type = ObjectiveType::DestroyVehicle;
        o2.targetCount = 3;
        o2.description = "Wreck 3 Zaibatsu Z-Types";
        m.objectives.push_back(o2);

        m_missions.push_back(m);
    }

    // 4. Yakuza Green Mission
    {
        Mission m;
        m.id = "y_green_01";
        m.title = "Dockside Honor";
        m.briefing = "Johnny Zoo speaking. Zaibatsu spies are infiltrating our container docks. Eliminate 4 of their scouts.";
        m.faction = GangFaction::Yakuza;
        m.tier = MissionTier::Green;
        m.minRespect = -20.0f;
        m.cashReward = 15000;
        m.respectReward = 30.0f;
        m.timeLimit = 200.0f;

        MissionObjective o1;
        o1.type = ObjectiveType::KillTarget;
        o1.targetCount = 4;
        o1.description = "Assassinate 4 Zaibatsu Scouts at the Docks";
        m.objectives.push_back(o1);

        m_missions.push_back(m);
    }
}

void MissionSystem::update(float dt) {
    if (m_activeMission) {
        if (m_activeMission->timeLimit > 0.0f) {
            m_activeMission->timeRemaining -= dt;
            if (m_activeMission->timeRemaining <= 0.0f) {
                failCurrentMission("Time Expired!");
                return;
            }
        }
    }

    if (m_killFrenzy.isActive) {
        m_killFrenzy.timeRemaining -= dt;
        if (m_killFrenzy.timeRemaining <= 0.0f) {
            m_killFrenzy.isActive = false;
            m_killFrenzy.isCompleted = (m_killFrenzy.currentKills >= m_killFrenzy.targetKills);
        }
    }
}

bool MissionSystem::startMission(const std::string& missionId, float currentRespect) {
    for (auto& m : m_missions) {
        if (m.id == missionId && m.status == MissionStatus::Available) {
            if (currentRespect >= m.minRespect) {
                m.status = MissionStatus::Active;
                m.timeRemaining = m.timeLimit;
                m.currentObjectiveIndex = 0;
                for (auto& obj : m.objectives) {
                    obj.currentCount = 0;
                    obj.isCompleted = false;
                }
                m_activeMission = &m;
                return true;
            }
        }
    }
    return false;
}

void MissionSystem::failCurrentMission(const std::string& reason) {
    if (m_activeMission) {
        m_activeMission->status = MissionStatus::Failed;
        m_activeMission = nullptr;
    }
}

void MissionSystem::completeCurrentMission() {
    if (m_activeMission) {
        m_activeMission->status = MissionStatus::Completed;
        m_activeMission = nullptr;
    }
}

void MissionSystem::startKillFrenzy(const KillFrenzy& frenzy) {
    m_killFrenzy = frenzy;
    m_killFrenzy.isActive = true;
    m_killFrenzy.currentKills = 0;
    m_killFrenzy.timeRemaining = frenzy.duration;
}

void MissionSystem::onKillScored(WeaponType weaponUsed) {
    if (m_killFrenzy.isActive) {
        if (m_killFrenzy.weapon == weaponUsed || m_killFrenzy.weapon == WeaponType::Fists) {
            m_killFrenzy.currentKills++;
            if (m_killFrenzy.currentKills >= m_killFrenzy.targetKills) {
                m_killFrenzy.isActive = false;
                m_killFrenzy.isCompleted = true;
            }
        }
    }

    if (m_activeMission && m_activeMission->currentObjectiveIndex < m_activeMission->objectives.size()) {
        auto& obj = m_activeMission->objectives[m_activeMission->currentObjectiveIndex];
        if (obj.type == ObjectiveType::KillTarget) {
            obj.currentCount++;
            if (obj.currentCount >= obj.targetCount) {
                obj.isCompleted = true;
                m_activeMission->currentObjectiveIndex++;
                if (m_activeMission->currentObjectiveIndex >= m_activeMission->objectives.size()) {
                    completeCurrentMission();
                }
            }
        }
    }
}

void MissionSystem::onVehicleDestroyed(VehicleType vehicleType) {
    if (m_activeMission && m_activeMission->currentObjectiveIndex < m_activeMission->objectives.size()) {
        auto& obj = m_activeMission->objectives[m_activeMission->currentObjectiveIndex];
        if (obj.type == ObjectiveType::DestroyVehicle) {
            obj.currentCount++;
            if (obj.currentCount >= obj.targetCount) {
                obj.isCompleted = true;
                m_activeMission->currentObjectiveIndex++;
                if (m_activeMission->currentObjectiveIndex >= m_activeMission->objectives.size()) {
                    completeCurrentMission();
                }
            }
        }
    }
}

void MissionSystem::onPlayerArrivedAtLocation(const Vec2& pos, float radius) {
    if (m_activeMission && m_activeMission->currentObjectiveIndex < m_activeMission->objectives.size()) {
        auto& obj = m_activeMission->objectives[m_activeMission->currentObjectiveIndex];
        if (obj.type == ObjectiveType::DeliverToPoint) {
            if (pos.distanceTo(obj.targetLocation) <= radius) {
                obj.isCompleted = true;
                m_activeMission->currentObjectiveIndex++;
                if (m_activeMission->currentObjectiveIndex >= m_activeMission->objectives.size()) {
                    completeCurrentMission();
                }
            }
        }
    }
}

const Mission* MissionSystem::findAvailableMission(GangFaction faction, MissionTier tier, float currentRespect) const {
    for (const auto& m : m_missions) {
        if (m.faction == faction && m.tier == tier && m.status == MissionStatus::Available && currentRespect >= m.minRespect) {
            return &m;
        }
    }
    return nullptr;
}

} // namespace gta2
