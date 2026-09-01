#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "core/Math.hpp"

using namespace gta2;

TEST_CASE("Vec2 Basic Arithmetic & Geometry", "[math]") {
    Vec2 a(3.0f, 4.0f);
    Vec2 b(1.0f, 2.0f);

    SECTION("Addition & Subtraction") {
        Vec2 add = a + b;
        REQUIRE(add.x == Catch::Approx(4.0f));
        REQUIRE(add.y == Catch::Approx(6.0f));

        Vec2 sub = a - b;
        REQUIRE(sub.x == Catch::Approx(2.0f));
        REQUIRE(sub.y == Catch::Approx(2.0f));
    }

    SECTION("Length & Normalization") {
        REQUIRE(a.length() == Catch::Approx(5.0f));
        REQUIRE(a.lengthSq() == Catch::Approx(25.0f));

        Vec2 norm = a.normalized();
        REQUIRE(norm.length() == Catch::Approx(1.0f));
        REQUIRE(norm.x == Catch::Approx(0.6f));
        REQUIRE(norm.y == Catch::Approx(0.8f));
    }

    SECTION("Dot & Cross Product") {
        REQUIRE(a.dot(b) == Catch::Approx(11.0f));
        REQUIRE(a.cross(b) == Catch::Approx(2.0f));
    }

    SECTION("Rotation") {
        Vec2 right(1.0f, 0.0f);
        Vec2 up = right.rotated(PI * 0.5f);
        REQUIRE(up.x == Catch::Approx(0.0f).margin(0.0001f));
        REQUIRE(up.y == Catch::Approx(1.0f).margin(0.0001f));
    }
}

TEST_CASE("Rect2D Collision & Bounds", "[math]") {
    Rect2D r1(0.0f, 0.0f, 100.0f, 100.0f);
    Rect2D r2(50.0f, 50.0f, 100.0f, 100.0f);
    Rect2D r3(200.0f, 200.0f, 50.0f, 50.0f);

    REQUIRE(r1.intersects(r2) == true);
    REQUIRE(r1.intersects(r3) == false);

    REQUIRE(r1.contains(Vec2(50.0f, 50.0f)) == true);
    REQUIRE(r1.contains(Vec2(150.0f, 150.0f)) == false);
}

TEST_CASE("Angle Normalization", "[math]") {
    REQUIRE(normalizeAngle(PI * 3.0f) == Catch::Approx(PI).margin(0.001f));
    REQUIRE(normalizeAngle(-PI * 3.0f) == Catch::Approx(-PI).margin(0.001f));
}
