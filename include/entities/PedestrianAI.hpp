#pragma once

#include "../core/Types.hpp"
#include "../core/Math.hpp"
#include "../core/Random.hpp"

namespace gta2 {

class Pedestrian;
class CityMap;

class PedestrianAI {
public:
    PedestrianAI() = default;

    void update(Pedestrian& ped, const CityMap& map, float dt, FastRandom& rng);

    void onGunshotHeard(Pedestrian& ped, const Vec2& gunshotPos);
    void onDamaged(Pedestrian& ped, const Vec2& attackerPos, EntityID attackerId);

private:
    void updateWander(Pedestrian& ped, const CityMap& map, float dt, FastRandom& rng);
    void updateFlee(Pedestrian& ped, float dt);
    void updateHostile(Pedestrian& ped, float dt);
    void updatePolice(Pedestrian& ped, float dt);

    float m_decisionTimer = 0.0f;
    Vec2 m_targetPos = {0.0f, 0.0f};
    EntityID m_targetEntityId = INVALID_ENTITY_ID;
};

} // namespace gta2
