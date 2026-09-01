#include "entities/PedestrianAI.hpp"
#include "entities/Pedestrian.hpp"
#include "world/CityMap.hpp"

namespace gta2 {

void PedestrianAI::update(Pedestrian& ped, const CityMap& map, float dt, FastRandom& rng) {
    if (ped.isDead() || ped.isInVehicle()) {
        return;
    }

    switch (ped.getState()) {
        case PedestrianState::Idle:
        case PedestrianState::Wandering:
            updateWander(ped, map, dt, rng);
            break;
        case PedestrianState::FleeingPanic:
            updateFlee(ped, dt);
            break;
        case PedestrianState::HostileAttacking:
            updateHostile(ped, dt);
            break;
        case PedestrianState::Arresting:
            updatePolice(ped, dt);
            break;
        default:
            break;
    }
}

void PedestrianAI::updateWander(Pedestrian& ped, const CityMap& map, float dt, FastRandom& rng) {
    m_decisionTimer -= dt;
    if (m_decisionTimer <= 0.0f) {
        m_decisionTimer = rng.nextFloat(2.0f, 5.0f);
        if (rng.nextBool(0.7f)) {
            // Pick a direction on sidewalk or wander
            Vec2 dir = rng.nextDirection();
            ped.setMoveDirection(dir);
            ped.setRotation(dir.angle());
            ped.setWalkSpeed(rng.nextFloat(60.0f, 90.0f));
            ped.setState(PedestrianState::Wandering);
        } else {
            ped.setMoveDirection({0.0f, 0.0f});
            ped.setState(PedestrianState::Idle);
        }
    }
}

void PedestrianAI::updateFlee(Pedestrian& ped, float dt) {
    m_decisionTimer -= dt;
    ped.setWalkSpeed(160.0f); // Fast panic sprint
    if (m_decisionTimer <= 0.0f) {
        // Return to normal wandering after fleeing
        ped.setState(PedestrianState::Wandering);
    }
}

void PedestrianAI::updateHostile(Pedestrian& ped, float dt) {
    // Attack target entity
    Vec2 toTarget = m_targetPos - ped.getPosition();
    float dist = toTarget.length();

    ped.setRotation(toTarget.angle());

    if (dist > 200.0f) {
        ped.setMoveDirection(toTarget.normalized());
        ped.setWalkSpeed(140.0f);
        ped.setWantsToAttack(false);
    } else {
        ped.setMoveDirection({0.0f, 0.0f});
        ped.setWantsToAttack(true);
    }
}

void PedestrianAI::updatePolice(Pedestrian& ped, float dt) {
    // Law enforcement pursuit
    Vec2 toTarget = m_targetPos - ped.getPosition();
    float dist = toTarget.length();

    ped.setRotation(toTarget.angle());
    ped.setWalkSpeed(150.0f);

    if (dist > 80.0f) {
        ped.setMoveDirection(toTarget.normalized());
        ped.setWantsToAttack(false);
    } else {
        ped.setMoveDirection({0.0f, 0.0f});
        ped.setWantsToAttack(true); // Nightstick arrest or pistol shot
    }
}

void PedestrianAI::onGunshotHeard(Pedestrian& ped, const Vec2& gunshotPos) {
    if (ped.getRole() == PedestrianRole::Civilian) {
        // Civilians flee in opposite direction
        Vec2 fleeDir = (ped.getPosition() - gunshotPos).normalized();
        ped.setMoveDirection(fleeDir);
        ped.setRotation(fleeDir.angle());
        ped.setState(PedestrianState::FleeingPanic);
        m_decisionTimer = 6.0f;
    } else if (ped.getRole() == PedestrianRole::PoliceFoot ||
               ped.getRole() == PedestrianRole::SwatOfficer) {
        // Police investigate gunshot source
        m_targetPos = gunshotPos;
        ped.setState(PedestrianState::Arresting);
    }
}

void PedestrianAI::onDamaged(Pedestrian& ped, const Vec2& attackerPos, EntityID attackerId) {
    if (ped.getRole() == PedestrianRole::Civilian) {
        Vec2 fleeDir = (ped.getPosition() - attackerPos).normalized();
        ped.setMoveDirection(fleeDir);
        ped.setRotation(fleeDir.angle());
        ped.setState(PedestrianState::FleeingPanic);
        m_decisionTimer = 8.0f;
    } else {
        // Armed gangs and police retaliate
        m_targetPos = attackerPos;
        m_targetEntityId = attackerId;
        ped.setState(PedestrianState::HostileAttacking);
    }
}

} // namespace gta2
