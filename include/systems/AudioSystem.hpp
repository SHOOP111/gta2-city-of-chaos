#pragma once

#include <memory>
#include "../audio/ProceduralAudio.hpp"
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem() = default;

    void initialize();
    void shutdown();

    void playSFX(SoundEffectType type, float volume = 1.0f);
    void playPositionalSFX(SoundEffectType type, const Vec2& soundPos, const Vec2& listenerPos, float volume = 1.0f);

    void setMasterVolume(float volume);
    void setSFXVolume(float volume);

    void update(float dt);

private:
    ProceduralAudio m_audioEngine;
    float m_sfxVolume = 1.0f;
};

} // namespace gta2
