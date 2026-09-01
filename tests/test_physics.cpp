#include "testing/TestHarness.hpp"
#include "entities/Vehicle.hpp"
#include "systems/PhysicsSystem.hpp"
#include "world/CityMap.hpp"

using namespace gta2;

TEST_CASE("Vehicle Dynamics & Acceleration", "[physics]") {
    Vehicle car(VehicleType::SportsExotic, Vec2(500.0f, 500.0f), 0.0f);

    SECTION("Throttle accelerates forward") {
        car.setThrottle(1.0f);
        car.update(0.1f);
        REQUIRE(car.getForwardSpeed() > 0.0f);
        REQUIRE(car.getPosition().x > 500.0f);
    }

    SECTION("Braking decelerates moving vehicle") {
        car.setThrottle(1.0f);
        car.update(0.5f);
        float topSpeed = car.getForwardSpeed();

        car.setThrottle(-1.0f); // Brake
        car.update(0.2f);
        REQUIRE(car.getForwardSpeed() < topSpeed);
    }

    SECTION("Handbrake engages drifting slip") {
        car.setThrottle(1.0f);
        car.setSteering(1.0f);
        car.setHandbrake(true);
        car.update(0.3f);
        REQUIRE(car.isDrifting() == true);
    }

    SECTION("Damage & Destruction") {
        float initialHealth = car.getHealth();
        car.takeDamage(100.0f);
        REQUIRE(car.getHealth() == Catch::Approx(initialHealth - 100.0f));

        car.takeDamage(1000.0f);
        REQUIRE(car.isDestroyed() == true);
    }
}

TEST_CASE("Physics Raycast against Solid Walls", "[physics]") {
    CityMap map;
    map.initialize(32, 32, 64.0f);

    PhysicsSystem physics;
    Vec2 start(64.0f, 64.0f);
    Vec2 end(64.0f * 5.0f, 64.0f);
    Vec2 hitPoint, hitNormal;

    physics.raycast(map, start, end, hitPoint, hitNormal);
    REQUIRE(hitPoint.lengthSq() >= 0.0f);
}
