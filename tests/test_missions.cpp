#include <catch2/catch_test_macros.hpp>
#include "systems/MissionSystem.hpp"

using namespace gta2 {

TEST_CASE("Mission Lifecycle & Objectives", "[missions]") {
    MissionSystem missions;

    SECTION("Missions initialized properly") {
        REQUIRE(missions.getAllMissions().size() >= 4);
        REQUIRE(missions.hasActiveMission() == false);
    }

    SECTION("Mission start requires respect threshold") {
        bool started = missions.startMission("z_green_01", -50.0f); // Respect too low
        REQUIRE(started == false);

        started = missions.startMission("z_green_01", 0.0f); // Meets threshold
        REQUIRE(started == true);
        REQUIRE(missions.hasActiveMission() == true);
    }

    SECTION("Kill Frenzy triggers and counts kills") {
        KillFrenzy f;
        f.title = "ROCKET HAVOC";
        f.weapon = WeaponType::RocketLauncher;
        f.targetKills = 5;
        f.duration = 60.0f;

        missions.startKillFrenzy(f);
        REQUIRE(missions.getKillFrenzy().isActive == true);

        for (int i = 0; i < 5; ++i) {
            missions.onKillScored(WeaponType::RocketLauncher);
        }

        REQUIRE(missions.getKillFrenzy().isCompleted == true);
    }
}

} // namespace gta2
