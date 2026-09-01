#include <catch2/catch_test_macros.hpp>
#include "core/Application.hpp"
#include "core/Engine.hpp"

using namespace gta2;

TEST_CASE("Full Headless Game Loop & Engine Simulation", "[headless]") {
    Engine engine;
    engine.initialize(true); // Headless mode

    SECTION("Engine initializes with Player, Map, and Traffic") {
        REQUIRE(engine.getPlayer().getHealth() > 0.0f);
        REQUIRE(engine.getMap().getWidth() > 0);
        REQUIRE(engine.getVehicles().size() > 0);
        REQUIRE(engine.getPedestrians().size() > 0);
    }

    SECTION("60-tick simulation runs smoothly") {
        uint64_t initialTick = engine.getTickCount();

        for (int i = 0; i < 60; ++i) {
            engine.handlePlayerInput(
                Vec2(0.0f, 1.0f), // Move forward
                false,            // Attack
                false,            // Enter/exit
                false,            // Handbrake
                false,            // Special
                false, false, false,
                1.0f / 60.0f
            );
            engine.update(1.0f / 60.0f);
        }

        REQUIRE(engine.getTickCount() == initialTick + 60);
        REQUIRE(engine.getPlayer().getPosition().y < engine.getMap().getWorldHeight());
    }

    SECTION("Vehicle Carjacking Simulation") {
        // Spawn a vehicle right in front of player
        Vec2 playerPos = engine.getPlayer().getPosition();
        auto v = engine.spawnVehicle(VehicleType::Sedan, playerPos + Vec2(10.0f, 10.0f));

        // Trigger vehicle entry
        engine.handlePlayerInput(
            Vec2(0.0f, 0.0f),
            false,
            true, // Enter vehicle action
            false,
            false,
            false, false, false,
            1.0f / 60.0f
        );
        engine.update(1.0f / 60.0f);

        REQUIRE(engine.getPlayer().isInVehicle() == true);
        REQUIRE(engine.getPlayer().getCurrentVehicleId() == v->getID());
    }
}
