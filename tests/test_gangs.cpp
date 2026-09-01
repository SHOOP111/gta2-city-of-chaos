#include "testing/TestHarness.hpp"
#include "systems/GangSystem.hpp"

using namespace gta2;

TEST_CASE("Gang Respect System Dynamics", "[gangs]") {
    GangSystem gangSystem;

    SECTION("Initial Respect is Zero") {
        REQUIRE(gangSystem.getRespect(GangFaction::Zaibatsu) == Catch::Approx(0.0f));
        REQUIRE(gangSystem.getRespect(GangFaction::Loonies) == Catch::Approx(0.0f));
        REQUIRE(gangSystem.getRespect(GangFaction::Yakuza) == Catch::Approx(0.0f));
    }

    SECTION("Killing a Zaibatsu member boosts Loonies and Yakuza respect") {
        gangSystem.onGangMemberKilled(GangFaction::Zaibatsu, GangFaction::None);
        REQUIRE(gangSystem.getRespect(GangFaction::Zaibatsu) < 0.0f);
        REQUIRE(gangSystem.getRespect(GangFaction::Loonies) > 0.0f);
        REQUIRE(gangSystem.getRespect(GangFaction::Yakuza) > 0.0f);
    }

    SECTION("Mission Completion Rewards Employer and Penalizes Target") {
        gangSystem.onMissionCompleted(GangFaction::Zaibatsu, GangFaction::Loonies, 30.0f);
        REQUIRE(gangSystem.getRespect(GangFaction::Zaibatsu) == Catch::Approx(30.0f));
        REQUIRE(gangSystem.getRespect(GangFaction::Loonies) < 0.0f);
    }

    SECTION("Mission Tier Thresholds") {
        gangSystem.setRespect(GangFaction::Zaibatsu, -10.0f);
        REQUIRE(gangSystem.canAccessMissionTier(GangFaction::Zaibatsu, MissionTier::Green) == true);
        REQUIRE(gangSystem.canAccessMissionTier(GangFaction::Zaibatsu, MissionTier::Yellow) == false);
        REQUIRE(gangSystem.canAccessMissionTier(GangFaction::Zaibatsu, MissionTier::Red) == false);

        gangSystem.setRespect(GangFaction::Zaibatsu, 35.0f);
        REQUIRE(gangSystem.canAccessMissionTier(GangFaction::Zaibatsu, MissionTier::Yellow) == true);

        gangSystem.setRespect(GangFaction::Zaibatsu, 75.0f);
        REQUIRE(gangSystem.canAccessMissionTier(GangFaction::Zaibatsu, MissionTier::Red) == true);
    }
}
