#pragma once

#include "Entity.hpp"
#include "PedestrianAI.hpp"
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

class Pedestrian : public Entity {
public:
    Pedestrian(
        PedestrianRole role,
        GangFaction gang,
        const Vec2& startPos,
        WeaponType weapon = WeaponType::Fists
    );

    void update(float dt) override;

    [[nodiscard]] PedestrianRole getRole() const noexcept { return m_role; }
    [[nodiscard]] GangFaction getGang() const noexcept { return m_gang; }
    [[nodiscard]] PedestrianState getState() const noexcept { return m_state; }
    void setState(PedestrianState state) noexcept { m_state = state; }

    [[nodiscard]] WeaponType getEquippedWeapon() const noexcept { return m_equippedWeapon; }
    void setEquippedWeapon(WeaponType weapon) noexcept { m_equippedWeapon = weapon; }

    [[nodiscard]] float getArmor() const noexcept { return m_armor; }
    void setArmor(float armor) noexcept { m_armor = std::clamp(armor, 0.0f, 100.0f); }

    [[nodiscard]] ColorRGBA getShirtColor() const noexcept { return m_shirtColor; }
    [[nodiscard]] ColorRGBA getPantsColor() const noexcept { return m_pantsColor; }

    [[nodiscard]] EntityID getCurrentVehicleId() const noexcept { return m_currentVehicleId; }
    void setCurrentVehicleId(EntityID id) noexcept { m_currentVehicleId = id; }
    [[nodiscard]] bool isInVehicle() const noexcept { return m_currentVehicleId != INVALID_ENTITY_ID; }

    [[nodiscard]] PedestrianAI& getAI() noexcept { return m_ai; }
    [[nodiscard]] const PedestrianAI& getAI() const noexcept { return m_ai; }

    void takeDamage(float amount) override;

    // Movement & Intent
    void setMoveDirection(const Vec2& dir) noexcept { m_moveDirection = dir; }
    [[nodiscard]] const Vec2& getMoveDirection() const noexcept { return m_moveDirection; }
    void setWalkSpeed(float speed) noexcept { m_walkSpeed = speed; }
    [[nodiscard]] float getWalkSpeed() const noexcept { return m_walkSpeed; }

    // Weapon Attack Trigger
    [[nodiscard]] bool wantsToAttack() const noexcept { return m_wantsToAttack; }
    void setWantsToAttack(bool attack) noexcept { m_wantsToAttack = attack; }

    [[nodiscard]] float getAttackCooldown() const noexcept { return m_attackCooldown; }
    void setAttackCooldown(float cd) noexcept { m_attackCooldown = cd; }

    [[nodiscard]] bool isBurning() const noexcept { return m_isBurning; }
    void setBurning(bool burning) noexcept { m_isBurning = burning; }

protected:
    void onKilled() override;

    PedestrianRole m_role = PedestrianRole::Civilian;
    GangFaction m_gang = GangFaction::None;
    PedestrianState m_state = PedestrianState::Wandering;
    WeaponType m_equippedWeapon = WeaponType::Fists;

    float m_armor = 0.0f;
    float m_walkSpeed = 90.0f;
    Vec2 m_moveDirection = {0.0f, 0.0f};

    ColorRGBA m_shirtColor = {200, 200, 200, 255};
    ColorRGBA m_pantsColor = {50, 50, 80, 255};

    EntityID m_currentVehicleId = INVALID_ENTITY_ID;
    PedestrianAI m_ai;

    bool m_wantsToAttack = false;
    float m_attackCooldown = 0.0f;
    bool m_isBurning = false;
    float m_burnTimer = 0.0f;
};

} // namespace gta2
