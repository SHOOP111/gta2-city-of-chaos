#pragma once

#include <vector>
#include <cstdint>
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

enum class SoundEffectType : uint8_t {
    PistolShot = 0,
    SMGShot,
    ShotgunBlast,
    AssaultRifleShot,
    RocketLaunch,
    ExplosionLarge,
    ExplosionSmall,
    FlamethrowerLoop,
    ElectroZap,
    CarEngineRev,
    TireScreech,
    CarHorn,
    CarCrash,
    PoliceSiren,
    PedScream,
    MoneyPickup,
    RespectBonus,
    MissionPassed,
    MissionFailed,
    KillFrenzyStart,
    RadioChirp
};

class ProceduralAudio {
public:
    ProceduralAudio();
    ~ProceduralAudio();

    void initialize();
    void shutdown();

    // Generates raw PCM wave samples (16-bit signed mono, 44100Hz)
    static std::vector<int16_t> generateGunshotSample(float pitch = 1.0f, float duration = 0.25f);
    static std::vector<int16_t> generateExplosionSample(float duration = 1.2f);
    static std::vector<int16_t> generateTireScreechSample(float duration = 0.4f);
    static std::vector<int16_t> generateSirenSample(float duration = 1.0f);
    static std::vector<int16_t> generateCoinChimeSample(float duration = 0.3f);
    static std::vector<int16_t> generateRetroBeepSample(float freq, float duration = 0.15f);

    void playSound(SoundEffectType type, float volume = 1.0f, float pitch = 1.0f);
    void playPositionalSound(SoundEffectType type, const Vec2& soundPos, const Vec2& listenerPos, float volume = 1.0f);

    void setMasterVolume(float vol) noexcept { m_masterVolume = std::clamp(vol, 0.0f, 1.0f); }
    [[nodiscard]] float getMasterVolume() const noexcept { return m_masterVolume; }

    void update(float dt);

private:
    float m_masterVolume = 0.8f;
    bool m_initialized = false;
};

} // namespace gta2
