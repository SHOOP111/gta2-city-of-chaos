#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "systems/WeaponSystem.hpp"
#include "entities/Projectile.hpp"

using namespace gta2;

TEST_CASE("Weapon System & Arsenal", "[weapons]") {
    WeaponSystem weapons;

    SECTION("Rocket Launcher is Explosive with High Damage") {
        const auto& r傷 = weapons.getStats(WeaponType::RocketLauncher);
        REQUIRE(r傷.damage >= 200.0f);
        REQUIRE(r傷.createsExplosion == true);
        REQUIRE(r傷.explosionRadius > 50.0f);
    }

    SECTION("Assault Rifle has Rapid Fire and Range") {
        const auto& ar = weapons.getStats(WeaponType::AssaultRifle);
        REQUIRE(ar.isAutomatic == true);
        REQUIRE(ar.fireRate >= 6.0f);
        REQUIRE(ar.range >= 500.0f);
    }

    SECTION("Projectile Movement & Lifetime") {
        Projectile bullet(
            WeaponType::Pistol,
            1,
            Vec2(100.0f, 100.0f),
            Vec2(1.0f, 0.0f),
            500.0f,
            25.0f,
            400.0f
        );

        bullet.update(0.1f);
        REQUIRE(bullet.getPosition().x == Catch::Approx(150.0f));
        REQUIRE(bullet.isActive() == true);

        // Exceed range
        bullet.update(1.0f);
        REQUIRE(bullet.isActive() == false);
    }
}
