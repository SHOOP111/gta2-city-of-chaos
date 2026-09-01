#pragma once

#include <vector>
#include <array>
#include "Pedestrian.hpp"
#include "Vehicle.hpp"
#include "../core/Config.hpp"

namespace gta2 {

struct WeaponSlot {
    WeaponType type = WeaponType::Fists;
    int ammo = 0;
    bool unlocked = false;
};

class Player : public Pedestrian {
public:
    explicit Player(const Vec2& startPos);

    void update(float dt) override;

    // Cash & Multiplier
    [[nodiscard]] int getCash() const noexcept { return m_cash; }
    void addCash(int amount) noexcept { m_cash += amount * m_multiplier; }
    void spendCash(int amount) noexcept { m_cash = std::max(0, m_cash - amount); }
    [[nodiscard]] int getMultiplier() const noexcept { return m_multiplier; }
    void incrementMultiplier() noexcept { m_multiplier = std::min(50, m_multiplier + 1); }

    // Weapon Inventory
    void unlockWeapon(WeaponType weapon, int ammo);
    void addAmmo(WeaponType weapon, int ammo);
    void selectNextWeapon();
    void selectPreviousWeapon();
    [[nodiscard]] int getWeaponAmmo(WeaponType weapon) const;
    [[nodiscard]] const std::vector<WeaponSlot>& getInventory() const noexcept { return m_inventory; }
    [[nodiscard]] size_t getSelectedWeaponIndex() const noexcept { return m_selectedWeaponIndex; }

    // Lives
    [[nodiscard]] int getLives() const noexcept { return m_lives; }
    void setLives(int lives) noexcept { m_lives = lives; }
    void loseLife() noexcept { if (m_lives > 0) --m_lives; }

    // Stats
    [[nodiscard]] int getKills() const noexcept { return m_kills; }
    void addKill() noexcept { ++m_kills; }
    [[nodiscard]] int getCarsStolen() const noexcept { return m_carsStolen; }
    void addCarStolen() noexcept { ++m_carsStolen; }

    // Respawn
    void respawn(const Vec2& pos);

private:
    int m_cash = 5000;
    int m_multiplier = 1;
    int m_lives = 3;
    int m_kills = 0;
    int m_carsStolen = 0;

    std::vector<WeaponSlot> m_inventory;
    size_t m_selectedWeaponIndex = 0;
};

} // namespace gta2
