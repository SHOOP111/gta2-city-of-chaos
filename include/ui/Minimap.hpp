#pragma once

#include <vector>
#include "../core/Math.hpp"
#include "../core/Types.hpp"

namespace gta2 {

struct RadarBlip {
    Vec2 worldPos;
    ColorRGBA color;
    float size = 4.0f;
    bool isBlinking = false;
    std::string label;
};

class Minimap {
public:
    Minimap() = default;

    void setScreenPosition(const Vec2& pos, float radius) {
        m_screenPos = pos;
        m_radius = radius;
    }

    void update(
        const Vec2& playerPos,
        const std::vector<RadarBlip>& blips,
        float dt
    ) {
        m_playerPos = playerPos;
        m_blips = blips;
        m_blinkTimer += dt;
    }

    [[nodiscard]] const Vec2& getScreenPosition() const noexcept { return m_screenPos; }
    [[nodiscard]] float getRadius() const noexcept { return m_radius; }
    [[nodiscard]] const Vec2& getPlayerPos() const noexcept { return m_playerPos; }
    [[nodiscard]] const std::vector<RadarBlip>& getBlips() const noexcept { return m_blips; }
    [[nodiscard]] bool getBlinkState() const noexcept { return static_cast<int>(m_blinkTimer * 4.0f) % 2 == 0; }

private:
    Vec2 m_screenPos = {120.0f, 120.0f};
    float m_radius = 90.0f;
    Vec2 m_playerPos = {0.0f, 0.0f};
    std::vector<RadarBlip> m_blips;
    float m_blinkTimer = 0.0f;
};

} // namespace gta2
