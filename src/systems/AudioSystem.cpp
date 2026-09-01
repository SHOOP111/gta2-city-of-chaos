#include "systems/AudioSystem.hpp"

namespace gta2 {

AudioSystem::AudioSystem() = default;

void AudioSystem::initialize() {
    m_audioEngine.initialize();
}

void AudioSystem::shutdown() {
    m_audioEngine.shutdown();
}

void AudioSystem::playSFX(SoundEffectType type, float volume) {
    m_audioEngine.playSound(type, volume * m_sfxVolume);
}

void AudioSystem::playPositionalSFX(SoundEffectType type, const Vec2& soundPos, const Vec2& listenerPos, float volume) {
    m_audioEngine.playPositionalSound(type, soundPos, listenerPos, volume * m_sfxVolume);
}

void AudioSystem::setMasterVolume(float volume) {
    m_audioEngine.setMasterVolume(volume);
}

void AudioSystem::setSFXVolume(float volume) {
    m_sfxVolume = volume;
}

void AudioSystem::update(float dt) {
    m_audioEngine.update(dt);
}

} // namespace gta2
