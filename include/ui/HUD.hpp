#pragma once

#include <string>
#include "../core/Types.hpp"
#include "../core/Math.hpp"
#include "../entities/Player.hpp"
#include "../systems/GangSystem.hpp"
#include "../systems/WantedSystem.hpp"
#include "../systems/MissionSystem.hpp"
#include "../systems/RadioSystem.hpp"
#include "Minimap.hpp"
#include "FloatingText.hpp"

namespace gta2 {

class HUD {
public:
    HUD();

    void update(
        const Player& player,
        const GangSystem& gangSystem,
        const WantedSystem& wantedSystem,
        const MissionSystem& missionSystem,
        const RadioSystem& radioSystem,
        float dt
    );

    void setScreenDimensions(int width, int height) noexcept {
        m_screenWidth = width;
        m_screenHeight = height;
    }

    [[nodiscard]] Minimap& getMinimap() noexcept { return m_minimap; }
    [[nodiscard]] const Minimap& getMinimap() const noexcept { return m_minimap; }

    [[nodiscard]] FloatingTextManager& getFloatingText() noexcept { return m_floatingText; }
    [[nodiscard]] const FloatingTextManager& getFloatingText() const noexcept { return m_floatingText; }

    // State getters for renderer
    [[nodiscard]] int getCash() const noexcept { return m_displayedCash; }
    [[nodiscard]] int getMultiplier() const noexcept { return m_displayedMultiplier; }
    [[nodiscard]] float getHealth() const noexcept { return m_displayedHealth; }
    [[nodiscard]] float getArmor() const noexcept { return m_displayedArmor; }
    [[nodiscard]] int getWantedStars() const noexcept { return m_wantedStars; }
    [[nodiscard]] int getLives() const noexcept { return m_lives; }
    [[nodiscard]] std::string getWeaponName() const noexcept { return m_weaponName; }
    [[nodiscard]] int getAmmo() const noexcept { return m_ammo; }
    [[nodiscard]] std::string getRadioStationName() const noexcept { return m_radioStationName; }
    [[nodiscard]] std::string getMissionTitle() const noexcept { return m_missionTitle; }
    [[nodiscard]] std::string getObjectiveText() const noexcept { return m_objectiveText; }
    [[nodiscard]] float getMissionTimeRemaining() const noexcept { return m_missionTimeRemaining; }

    [[nodiscard]] float getZaibatsuRespect() const noexcept { return m_zaibatsuRespect; }
    [[nodiscard]] float getLooniesRespect() const noexcept { return m_looniesRespect; }
    [[nodiscard]] float getYakuzaRespect() const noexcept { return m_yakuzaRespect; }

private:
    int m_screenWidth = 1280;
    int m_screenHeight = 720;

    int m_displayedCash = 0;
    int m_targetCash = 0;
    int m_displayedMultiplier = 1;
    float m_displayedHealth = 100.0f;
    float m_displayedArmor = 100.0f;
    int m_wantedStars = 0;
    int m_lives = 3;

    std::string m_weaponName = "Fists";
    int m_ammo = 0;
    std::string m_radioStationName = "Head Radio";

    std::string m_missionTitle;
    std::string m_objectiveText;
    float m_missionTimeRemaining = 0.0f;

    float m_zaibatsuRespect = 0.0f;
    float m_looniesRespect = 0.0f;
    float m_yakuzaRespect = 0.0f;

    Minimap m_minimap;
    FloatingTextManager m_floatingText;
};

} // namespace gta2
