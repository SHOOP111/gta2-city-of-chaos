#pragma once

#include <string>
#include <vector>
#include "../core/Math.hpp"

namespace gta2 {

struct FloatingMessage {
    std::string text;
    Vec2 worldPosition;
    ColorRGBA color = ColorRGBA::White();
    float life = 2.0f;
    float maxLife = 2.0f;
    float scale = 1.0f;
    float riseSpeed = 30.0f;
};

class FloatingTextManager {
public:
    FloatingTextManager() = default;

    void addMessage(const std::string& text, const Vec2& pos, const ColorRGBA& color, float scale = 1.0f, float duration = 2.0f) {
        FloatingMessage msg;
        msg.text = text;
        msg.worldPosition = pos;
        msg.color = color;
        msg.scale = scale;
        msg.life = duration;
        msg.maxLife = duration;
        m_messages.push_back(msg);
    }

    void update(float dt) {
        for (auto& msg : m_messages) {
            msg.worldPosition.y -= msg.riseSpeed * dt;
            msg.life -= dt;
            float alphaProgress = msg.life / msg.maxLife;
            msg.color.a = static_cast<uint8_t>(std::clamp(alphaProgress * 255.0f, 0.0f, 255.0f));
        }

        std::erase_if(m_messages, [](const FloatingMessage& msg) {
            return msg.life <= 0.0f;
        });
    }

    [[nodiscard]] const std::vector<FloatingMessage>& getMessages() const noexcept { return m_messages; }
    void clear() { m_messages.clear(); }

private:
    std::vector<FloatingMessage> m_messages;
};

} // namespace gta2
