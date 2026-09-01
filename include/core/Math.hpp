#pragma once

#include <cmath>
#include <algorithm>
#include <cstdint>

namespace gta2 {

constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = 2.0f * PI;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    constexpr Vec2() = default;
    constexpr Vec2(float x_, float y_) : x(x_), y(y_) {}

    constexpr Vec2 operator+(const Vec2& rhs) const noexcept { return {x + rhs.x, y + rhs.y}; }
    constexpr Vec2 operator-(const Vec2& rhs) const noexcept { return {x - rhs.x, y - rhs.y}; }
    constexpr Vec2 operator-() const noexcept { return {-x, -y}; }
    constexpr Vec2 operator*(float scalar) const noexcept { return {x * scalar, y * scalar}; }
    constexpr Vec2 operator/(float scalar) const noexcept {
        float inv = 1.0f / scalar;
        return {x * inv, y * inv};
    }

    Vec2& operator+=(const Vec2& rhs) noexcept {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs) noexcept {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vec2& operator*=(float scalar) noexcept {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    constexpr bool operator==(const Vec2& rhs) const noexcept {
        return (x == rhs.x && y == rhs.y);
    }

    [[nodiscard]] float lengthSq() const noexcept { return x * x + y * y; }
    [[nodiscard]] float length() const noexcept { return std::sqrt(lengthSq()); }

    [[nodiscard]] Vec2 normalized() const noexcept {
        float len = length();
        if (len > 0.00001f) {
            return *this / len;
        }
        return {0.0f, 0.0f};
    }

    [[nodiscard]] constexpr float dot(const Vec2& rhs) const noexcept {
        return x * rhs.x + y * rhs.y;
    }

    [[nodiscard]] constexpr float cross(const Vec2& rhs) const noexcept {
        return x * rhs.y - y * rhs.x;
    }

    [[nodiscard]] float angle() const noexcept {
        return std::atan2(y, x);
    }

    [[nodiscard]] float distanceTo(const Vec2& rhs) const noexcept {
        return (*this - rhs).length();
    }

    [[nodiscard]] float distanceSqTo(const Vec2& rhs) const noexcept {
        return (*this - rhs).lengthSq();
    }

    [[nodiscard]] Vec2 rotated(float radians) const noexcept {
        float c = std::cos(radians);
        float s = std::sin(radians);
        return {x * c - y * s, x * s + y * c};
    }

    [[nodiscard]] static Vec2 fromAngle(float radians, float length = 1.0f) noexcept {
        return {std::cos(radians) * length, std::sin(radians) * length};
    }

    [[nodiscard]] static Vec2 lerp(const Vec2& a, const Vec2& b, float t) noexcept {
        return a + (b - a) * std::clamp(t, 0.0f, 1.0f);
    }
};

struct Rect2D {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    constexpr Rect2D() = default;
    constexpr Rect2D(float x_, float y_, float w_, float h_)
        : x(x_), y(y_), width(w_), height(h_) {}

    [[nodiscard]] constexpr bool contains(const Vec2& pt) const noexcept {
        return (pt.x >= x && pt.x <= x + width && pt.y >= y && pt.y <= y + height);
    }

    [[nodiscard]] constexpr bool intersects(const Rect2D& rhs) const noexcept {
        return (x < rhs.x + rhs.width && x + width > rhs.x &&
                y < rhs.y + rhs.height && y + height > rhs.y);
    }

    [[nodiscard]] constexpr Vec2 center() const noexcept {
        return {x + width * 0.5f, y + height * 0.5f};
    }
};

struct ColorRGBA {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    constexpr ColorRGBA() = default;
    constexpr ColorRGBA(int r_, int g_, int b_, int a_ = 255)
        : r(static_cast<uint8_t>(r_)),
          g(static_cast<uint8_t>(g_)),
          b(static_cast<uint8_t>(b_)),
          a(static_cast<uint8_t>(a_)) {}

    static constexpr ColorRGBA Red() { return {255, 0, 0, 255}; }
    static constexpr ColorRGBA Green() { return {0, 255, 0, 255}; }
    static constexpr ColorRGBA Blue() { return {0, 0, 255, 255}; }
    static constexpr ColorRGBA White() { return {255, 255, 255, 255}; }
    static constexpr ColorRGBA Black() { return {0, 0, 0, 255}; }
    static constexpr ColorRGBA Yellow() { return {255, 255, 0, 255}; }
    static constexpr ColorRGBA Orange() { return {255, 140, 0, 255}; }
    static constexpr ColorRGBA Cyan() { return {0, 255, 255, 255}; }
    static constexpr ColorRGBA Purple() { return {180, 0, 255, 255}; }
    static constexpr ColorRGBA Gray() { return {128, 128, 128, 255}; }
    static constexpr ColorRGBA DarkGray() { return {45, 45, 45, 255}; }
};

inline float normalizeAngle(float radians) noexcept {
    while (radians > PI) radians -= TWO_PI;
    while (radians < -PI) radians += TWO_PI;
    return radians;
}

inline float angleDifference(float target, float current) noexcept {
    return normalizeAngle(target - current);
}

} // namespace gta2
