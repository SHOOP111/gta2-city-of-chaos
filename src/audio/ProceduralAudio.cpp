#include "audio/ProceduralAudio.hpp"
#include <cmath>
#include <random>

#ifndef HEADLESS_MODE
#include "raylib.h"
#endif

namespace gta2 {

ProceduralAudio::ProceduralAudio() = default;

ProceduralAudio::~ProceduralAudio() {
    shutdown();
}

void ProceduralAudio::initialize() {
#ifndef HEADLESS_MODE
    if (!m_initialized) {
        InitAudioDevice();
        m_initialized = true;
    }
#endif
}

void ProceduralAudio::shutdown() {
#ifndef HEADLESS_MODE
    if (m_initialized) {
        CloseAudioDevice();
        m_initialized = false;
    }
#endif
}

std::vector<int16_t> ProceduralAudio::generateGunshotSample(float pitch, float duration) {
    size_t sampleRate = 44100;
    size_t totalSamples = static_cast<size_t>(static_cast<float>(sampleRate) * duration);
    std::vector<int16_t> buffer(totalSamples, 0);

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> noiseDist(-1.0f, 1.0f);

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(sampleRate);
        float decay = std::exp(-t * (18.0f * pitch));
        float tone = std::sin(2.0f * 3.14159f * (180.0f * pitch) * t);
        float noise = noiseDist(gen);
        float sample = (0.7f * noise + 0.3f * tone) * decay;
        buffer[i] = static_cast<int16_t>(std::clamp(sample * 30000.0f, -32767.0f, 32767.0f));
    }
    return buffer;
}

std::vector<int16_t> ProceduralAudio::generateExplosionSample(float duration) {
    size_t sampleRate = 44100;
    size_t totalSamples = static_cast<size_t>(static_cast<float>(sampleRate) * duration);
    std::vector<int16_t> buffer(totalSamples, 0);

    std::mt19937 gen(1337);
    std::uniform_real_distribution<float> noiseDist(-1.0f, 1.0f);
    float lowPassState = 0.0f;

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(sampleRate);
        float decay = std::exp(-t * 3.2f);
        float noise = noiseDist(gen);
        // Low pass filter for heavy rumble
        lowPassState += (noise - lowPassState) * 0.15f;
        float subBass = std::sin(2.0f * 3.14159f * 55.0f * t) * 0.4f;
        float sample = (lowPassState * 0.8f + subBass) * decay;
        buffer[i] = static_cast<int16_t>(std::clamp(sample * 32000.0f, -32767.0f, 32767.0f));
    }
    return buffer;
}

std::vector<int16_t> ProceduralAudio::generateTireScreechSample(float duration) {
    size_t sampleRate = 44100;
    size_t totalSamples = static_cast<size_t>(static_cast<float>(sampleRate) * duration);
    std::vector<int16_t> buffer(totalSamples, 0);

    std::mt19937 gen(777);
    std::uniform_real_distribution<float> noiseDist(-1.0f, 1.0f);

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(sampleRate);
        float modulation = std::sin(2.0f * 3.14159f * 1200.0f * t) * 0.6f;
        float noise = noiseDist(gen) * 0.4f;
        float sample = (modulation + noise);
        buffer[i] = static_cast<int16_t>(std::clamp(sample * 16000.0f, -32767.0f, 32767.0f));
    }
    return buffer;
}

std::vector<int16_t> ProceduralAudio::generateSirenSample(float duration) {
    size_t sampleRate = 44100;
    size_t totalSamples = static_cast<size_t>(static_cast<float>(sampleRate) * duration);
    std::vector<int16_t> buffer(totalSamples, 0);

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(sampleRate);
        float freq = 600.0f + 300.0f * std::sin(2.0f * 3.14159f * 1.5f * t);
        float sample = std::sin(2.0f * 3.14159f * freq * t);
        buffer[i] = static_cast<int16_t>(sample * 20000.0f);
    }
    return buffer;
}

std::vector<int16_t> ProceduralAudio::generateCoinChimeSample(float duration) {
    size_t sampleRate = 44100;
    size_t totalSamples = static_cast<size_t>(static_cast<float>(sampleRate) * duration);
    std::vector<int16_t> buffer(totalSamples, 0);

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(sampleRate);
        float freq = (t < duration * 0.5f) ? 987.77f : 1318.51f; // B5 to E6 chime
        float decay = std::exp(-t * 8.0f);
        float sample = std::sin(2.0f * 3.14159f * freq * t) * decay;
        buffer[i] = static_cast<int16_t>(sample * 25000.0f);
    }
    return buffer;
}

std::vector<int16_t> ProceduralAudio::generateRetroBeepSample(float freq, float duration) {
    size_t sampleRate = 44100;
    size_t totalSamples = static_cast<size_t>(static_cast<float>(sampleRate) * duration);
    std::vector<int16_t> buffer(totalSamples, 0);

    for (size_t i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(sampleRate);
        float decay = std::exp(-t * 10.0f);
        float sample = (std::sin(2.0f * 3.14159f * freq * t) > 0.0f ? 1.0f : -1.0f) * 0.5f * decay; // Square wave
        buffer[i] = static_cast<int16_t>(sample * 18000.0f);
    }
    return buffer;
}

void ProceduralAudio::playSound(SoundEffectType type, float volume, float pitch) {
    // Sound dispatch
}

void ProceduralAudio::playPositionalSound(SoundEffectType type, const Vec2& soundPos, const Vec2& listenerPos, float volume) {
    float dist = soundPos.distanceTo(listenerPos);
    constexpr float maxAudibleDist = 1200.0f;
    if (dist < maxAudibleDist) {
        float falloff = 1.0f - (dist / maxAudibleDist);
        playSound(type, volume * falloff);
    }
}

void ProceduralAudio::update(float dt) {
    // Background audio tick
}

} // namespace gta2
