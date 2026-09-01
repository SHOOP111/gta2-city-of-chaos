#pragma once

#include "../core/Math.hpp"
#include "../core/Config.hpp"

namespace gta2 {

class Camera2D {
public:
    Camera2D() = default;

    void setTarget(const Vec2& target) noexcept {
        m_target = target;
    }

    void setPosition(const Vec2& pos) noexcept {
        m_position = pos;
        m_target = pos;
    }

    void setSpeed(float speed) noexcept {
        m_speed = speed;
    }

    void addTrauma(float trauma) noexcept {
        m_trauma = std::clamp(m_trauma + trauma, 0.0f, 1.0f);
    }

    void update(float dt, FastRandom& rng) {
        const auto& cfg = globalConfig();
        
        // Smooth target follow
        m_position = Vec2::lerp(m_position, m_target, dt * cfg.cameraSmoothSpeed);

        // Speed-dependent zoom out
        float targetZoom = cfg.cameraBaseZoom / (1.0f + m_speed * cfg.cameraSpeedZoomFactor);
        targetZoom = std::clamp(targetZoom, cfg.cameraMinZoom, cfg.cameraMaxZoom);
        m_zoom = std::lerp(m_zoom, targetZoom, dt * 3.0f);

        // Screen shake decay and displacement
        if (m_trauma > 0.0f) {
            float shakeAmount = m_trauma * m_trauma; // Non-linear shake feel
            m_shakeOffset.x = (rng.nextFloat(-1.0f, 1.0f)) * shakeAmount * 24.0f;
            m_shakeOffset.y = (rng.nextFloat(-1.0f, 1.0f)) * shakeAmount * 24.0f;
            m_trauma = std::max(0.0f, m_trauma - dt * 1.5f);
        } else {
            m_shakeOffset = {0.0f, 0.0f};
        }
    }

    [[nodiscard]] Vec2 getPosition() const noexcept { return m_position + m_shakeOffset; }
    [[nodiscard]] float getZoom() const noexcept { return m_zoom; }

    [[nodiscard]] Vec2 worldToScreen(const Vec2& worldPos, int screenW, int screenH) const noexcept {
        Vec2 camPos = getPosition();
        return {
            (worldPos.x - camPos.x) * m_zoom + static_cast<float>(screenW) * 0.5f,
            (worldPos.y - camPos.y) * m_zoom + static_cast<float>(screenH) * 0.5f
        };
    }

    [[nodiscard]] Vec2 screenToWorld(const Vec2& screenPos, int screenW, int screenH) const noexcept {
        Vec2 camPos = getPosition();
        return {
            (screenPos.x - static_cast<float>(screenW) * 0.5f) / m_zoom + camPos.x,
            (screenPos.y - static_cast<float>(screenH) * 0.5f) / m_zoom + camPos.y
        };
    }

    [[nodiscard]] Rect2D getVisibleWorldBounds(int screenW, int screenH) const noexcept {
        Vec2 topLeft = screenToWorld({0.0f, 0.0f}, screenW, screenH);
        Vec2 botRight = screenToWorld({static_cast<float>(screenW), static_cast<float>(screenH)}, screenW, screenH);
        return Rect2D{topLeft.x, topLeft.y, botRight.x - topLeft.x, botRight.y - topLeft.y};
    }

private:
    Vec2 m_position = {0.0f, 0.0f};
    Vec2 m_target = {0.0f, 0.0f};
    Vec2 m_shakeOffset = {0.0f, 0.0f};
    float m_speed = 0.0f;
    float m_zoom = 1.0f;
    float m_trauma = 0.0f;
};

} // namespace gta2
