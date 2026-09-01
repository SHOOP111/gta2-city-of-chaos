#pragma once

#include <algorithm>
#include "../core/Math.hpp"

namespace gta2 {

class DayNightCycle {
public:
    DayNightCycle() = default;

    void update(float dt) noexcept {
        m_timeOfDay += dt * m_timeScale;
        if (m_timeOfDay >= 24.0f) {
            m_timeOfDay -= 24.0f;
        }

        // Calculate ambient color and darkness
        // 06:00 = Dawn, 12:00 = Noon, 18:00 = Dusk, 24:00/00:00 = Midnight
        float hour = m_timeOfDay;
        if (hour >= 6.0f && hour < 8.0f) {
            // Dawn transition
            float t = (hour - 6.0f) / 2.0f;
            m_ambientLight = ColorRGBA(
                static_cast<uint8_t>(std::lerp(50.0f, 255.0f, t)),
                static_cast<uint8_t>(std::lerp(60.0f, 250.0f, t)),
                static_cast<uint8_t>(std::lerp(100.0f, 240.0f, t)),
                255
            );
            m_darkness = std::lerp(0.7f, 0.0f, t);
        } else if (hour >= 8.0f && hour < 17.0f) {
            // Full Daylight
            m_ambientLight = ColorRGBA(255, 255, 255, 255);
            m_darkness = 0.0f;
        } else if (hour >= 17.0f && hour < 20.0f) {
            // Sunset / Dusk
            float t = (hour - 17.0f) / 3.0f;
            m_ambientLight = ColorRGBA(
                static_cast<uint8_t>(std::lerp(255.0f, 60.0f, t)),
                static_cast<uint8_t>(std::lerp(230.0f, 50.0f, t)),
                static_cast<uint8_t>(std::lerp(200.0f, 110.0f, t)),
                255
            );
            m_darkness = std::lerp(0.0f, 0.75f, t);
        } else {
            // Night
            m_ambientLight = ColorRGBA(40, 45, 80, 255);
            m_darkness = 0.75f;
        }

        // Streetlights are on between 18:30 and 06:30
        m_streetlightsActive = (hour >= 18.5f || hour < 6.5f);
    }

    [[nodiscard]] float getTimeOfDay() const noexcept { return m_timeOfDay; }
    [[nodiscard]] ColorRGBA getAmbientLight() const noexcept { return m_ambientLight; }
    [[nodiscard]] float getDarkness() const noexcept { return m_darkness; }
    [[nodiscard]] bool areStreetlightsActive() const noexcept { return m_streetlightsActive; }

    void setTimeOfDay(float hour) noexcept {
        m_timeOfDay = std::clamp(hour, 0.0f, 24.0f);
    }

    void setTimeScale(float scale) noexcept {
        m_timeScale = scale;
    }

private:
    float m_timeOfDay = 12.0f; // Start at Noon
    float m_timeScale = 0.05f; // ~8 minutes real time = 24 game hours
    ColorRGBA m_ambientLight = ColorRGBA::White();
    float m_darkness = 0.0f;
    bool m_streetlightsActive = false;
};

} // namespace gta2
