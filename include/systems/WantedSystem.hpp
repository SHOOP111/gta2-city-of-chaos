#pragma once

#include <vector>
#include <memory>
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

class WantedSystem {
public:
    WantedSystem() = default;

    void update(float dt, const Vec2& playerPos, bool isInPlayerView);

    void addCrimePoints(int points);
    void clearWantedLevel();
    void setWantedLevel(WantedLevel level);

    [[nodiscard]] WantedLevel getWantedLevel() const noexcept { return m_wantedLevel; }
    [[nodiscard]] int getCrimePoints() const noexcept { return m_crimePoints; }
    [[nodiscard]] int getStarCount() const noexcept { return static_cast<int>(m_wantedLevel); }

    // Heat decay when hiding outside police search radius
    [[nodiscard]] float getHeatDecayProgress() const noexcept { return m_heatDecayTimer / m_heatDecayDuration; }
    [[nodiscard]] bool isEvading() const noexcept { return m_isEvading; }

    // Spawn parameters for Law Enforcement Dispatcher
    [[nodiscard]] int getMaxActiveCops() const noexcept;
    [[nodiscard]] int getMaxActiveCopCars() const noexcept;
    [[nodiscard]] bool shouldSpawnSwatVans() const noexcept;
    [[nodiscard]] bool shouldSpawnFbiAgents() const noexcept;
    [[nodiscard]] bool shouldSpawnArmyTanks() const noexcept;
    [[nodiscard]] bool shouldSpawnRoadblocks() const noexcept;

    // Respray shop interaction
    void onResprayUsed();

private:
    WantedLevel m_wantedLevel = WantedLevel::Clean;
    int m_crimePoints = 0;
    float m_heatDecayTimer = 0.0f;
    float m_heatDecayDuration = 15.0f;
    bool m_isEvading = false;
};

} // namespace gta2
