#include <catch2/catch_test_macros.hpp>
#include "systems/WantedSystem.hpp"

using namespace gta2;

TEST_CASE("Wanted Law Enforcement System", "[wanted]") {
    WantedSystem wanted;

    SECTION("Initial Wanted Level is Clean") {
        REQUIRE(wanted.getWantedLevel() == WantedLevel::Clean);
        REQUIRE(wanted.getStarCount() == 0);
    }

    SECTION("Crime Points Escalate Wanted Stars") {
        wanted.addCrimePoints(30);
        REQUIRE(wanted.getWantedLevel() == WantedLevel::Star1);
        REQUIRE(wanted.getStarCount() == 1);

        wanted.addCrimePoints(70);
        REQUIRE(wanted.getWantedLevel() == WantedLevel::Star2);
        REQUIRE(wanted.getStarCount() == 2);

        wanted.addCrimePoints(150);
        REQUIRE(wanted.getWantedLevel() == WantedLevel::Star3);
        REQUIRE(wanted.shouldSpawnSwatVans() == true);

        wanted.addCrimePoints(800);
        REQUIRE(wanted.getWantedLevel() == WantedLevel::Star6);
        REQUIRE(wanted.shouldSpawnArmyTanks() == true);
    }

    SECTION("Respray Clears Wanted Stars") {
        wanted.setWantedLevel(WantedLevel::Star4);
        wanted.onResprayUsed();
        REQUIRE(wanted.getWantedLevel() == WantedLevel::Clean);
        REQUIRE(wanted.getStarCount() == 0);
    }
}
