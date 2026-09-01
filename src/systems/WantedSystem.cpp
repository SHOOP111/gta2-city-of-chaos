#include "systems/WantedSystem.hpp"
#include <algorithm>

namespace gta2 {

void WantedSystem::update(float dt, const Vec2& playerPos, bool isInPlayerView) {
    if (m_wantedLevel == WantedLevel::Clean) {
        m_heatDecayTimer = 0.0f;
        m_isEvading = false;
        return;
    }

    if (!isInPlayerView) {
        // Player is out of sight, begin heat evasion decay
        m_isEvading = true;
        m_heatDecayTimer += dt;

        if (m_heatDecayTimer >= m_heatDecayDuration) {
            // Drop a star
            int currentLevel = static_cast<int>(m_wantedLevel);
            if (currentLevel > 0) {
                m_wantedLevel = static_cast<WantedLevel>(currentLevel - 1);
                m_heatDecayTimer = 0.0f;
                m_crimePoints = std::max(0, m_crimePoints - 100);
            }
        }
    } else {
        // In sight of police, reset evasion timer
        m_isEvading = false;
        m_heatDecayTimer = std::max(0.0f, m_heatDecayTimer - dt * 2.0f);
    }
}

void WantedSystem::addCrimePoints(int points) {
    m_crimePoints += points;
    m_heatDecayTimer = 0.0f;

    if (m_crimePoints >= 1000) {
        m_wantedLevel = WantedLevel::Star6;
    } else if (m_crimePoints >= 650) {
        m_wantedLevel = WantedLevel::Star5;
    } else if (m_crimePoints >= 400) {
        m_wantedLevel = WantedLevel::Star4;
    } else if (m_crimePoints >= 220) {
        m_wantedLevel = WantedLevel::Star3;
    } else if (m_crimePoints >= 90) {
        m_wantedLevel = WantedLevel::Star2;
    } else if (m_crimePoints >= 25) {
        m_wantedLevel = WantedLevel::Star1;
    }
}

void WantedSystem::clearWantedLevel() {
    m_wantedLevel = WantedLevel::Clean;
    m_crimePoints = 0;
    m_heatDecayTimer = 0.0f;
    m_isEvading = false;
}

void WantedSystem::setWantedLevel(WantedLevel level) {
    m_wantedLevel = level;
    m_heatDecayTimer = 0.0f;
}

void WantedSystem::onResprayUsed() {
    clearWantedLevel();
}

int WantedSystem::getMaxActiveCops() const noexcept {
    switch (m_wantedLevel) {
        case WantedLevel::Clean: return 0;
        case WantedLevel::Star1: return 2;
        case WantedLevel::Star2: return 4;
        case WantedLevel::Star3: return 6;
        case WantedLevel::Star4: return 8;
        case WantedLevel::Star5: return 10;
        case WantedLevel::Star6: return 12;
    }
    return 0;
}

int WantedSystem::getMaxActiveCopCars() const noexcept {
    switch (m_wantedLevel) {
        case WantedLevel::Clean: return 0;
        case WantedLevel::Star1: return 0;
        case WantedLevel::Star2: return 2;
        case WantedLevel::Star3: return 3;
        case WantedLevel::Star4: return 4;
        case WantedLevel::Star5: return 5;
        case WantedLevel::Star6: return 6;
    }
    return 0;
}

bool WantedSystem::shouldSpawnSwatVans() const noexcept {
    return m_wantedLevel >= WantedLevel::Star3;
}

bool WantedSystem::shouldSpawnFbiAgents() const noexcept {
    return m_wantedLevel >= WantedLevel::Star5;
}

bool WantedSystem::shouldSpawnArmyTanks() const noexcept {
    return m_wantedLevel >= WantedLevel::Star6;
}

bool WantedSystem::shouldSpawnRoadblocks() const noexcept {
    return m_wantedLevel >= WantedLevel::Star3;
}

} // namespace gta2
