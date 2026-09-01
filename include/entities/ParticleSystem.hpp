#pragma once

#include <vector>
#include "../core/Math.hpp"
#include "../core/Random.hpp"

namespace gta2 {

enum class ParticleType : uint8_t {
    Blood = 0,
    TireSkid,
    TireSmoke,
    GunSpark,
    ExplosionFire,
    ExplosionSmoke,
    FlamethrowerFlame,
    WaterSplash,
    GlassShards,
    BulletCasing
};

struct Particle {
    Vec2 position;
    Vec2 velocity;
    ColorRGBA color;
    float size = 4.0f;
    float initialSize = 4.0f;
    float life = 1.0f;
    float maxLife = 1.0f;
    float rotation = 0.0f;
    float angularVelocity = 0.0f;
    ParticleType type = ParticleType::GunSpark;
    bool isPersistent = false; // E.g. Skid marks and blood stay on ground
};

class ParticleSystem {
public:
    ParticleSystem() = default;

    void update(float dt) {
        for (auto& p : m_particles) {
            p.position += p.velocity * dt;
            p.rotation += p.angularVelocity * dt;
            p.life -= dt;

            // Fade size and color alpha
            float progress = 1.0f - (p.life / p.maxLife);
            if (p.type == ParticleType::TireSmoke || p.type == ParticleType::ExplosionSmoke) {
                p.size = p.initialSize * (1.0f + progress * 2.5f);
                p.velocity *= std::max(0.0f, 1.0f - dt * 2.0f);
            } else if (p.type == ParticleType::ExplosionFire || p.type == ParticleType::FlamethrowerFlame) {
                p.size = p.initialSize * (1.0f + progress * 1.5f);
            } else if (p.type == ParticleType::Blood || p.type == ParticleType::TireSkid) {
                p.velocity *= std::max(0.0f, 1.0f - dt * 5.0f);
            }

            p.color.a = static_cast<uint8_t>(std::clamp(255.0f * (p.life / p.maxLife), 0.0f, 255.0f));
        }

        // Clean up expired particles
        std::erase_if(m_particles, [](const Particle& p) {
            return p.life <= 0.0f;
        });
    }

    void emit(const Particle& p) {
        if (m_particles.size() < 2000) {
            m_particles.push_back(p);
        }
    }

    void emitBloodSplatter(const Vec2& pos, const Vec2& dir, FastRandom& rng, int count = 8) {
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.position = pos;
            float spread = rng.nextFloat(-0.8f, 0.8f);
            Vec2 vDir = dir.rotated(spread).normalized();
            p.velocity = vDir * rng.nextFloat(30.0f, 120.0f);
            p.color = ColorRGBA(180 + rng.nextInt(0, 50), 10, 15, 255);
            p.size = rng.nextFloat(3.0f, 6.0f);
            p.initialSize = p.size;
            p.life = rng.nextFloat(4.0f, 10.0f); // Stays on pavement
            p.maxLife = p.life;
            p.type = ParticleType::Blood;
            emit(p);
        }
    }

    void emitExplosion(const Vec2& pos, FastRandom& rng) {
        // Fire burst
        for (int i = 0; i < 24; ++i) {
            Particle p;
            p.position = pos;
            p.velocity = rng.nextDirection() * rng.nextFloat(40.0f, 220.0f);
            p.color = ColorRGBA(255, rng.nextInt(80, 220), 20, 255);
            p.size = rng.nextFloat(12.0f, 24.0f);
            p.initialSize = p.size;
            p.life = rng.nextFloat(0.4f, 0.8f);
            p.maxLife = p.life;
            p.type = ParticleType::ExplosionFire;
            emit(p);
        }
        // Smoke plumes
        for (int i = 0; i < 18; ++i) {
            Particle p;
            p.position = pos + rng.nextDirection() * rng.nextFloat(0.0f, 20.0f);
            p.velocity = rng.nextDirection() * rng.nextFloat(20.0f, 80.0f);
            uint8_t grey = static_cast<uint8_t>(rng.nextInt(50, 100));
            p.color = ColorRGBA(grey, grey, grey, 200);
            p.size = rng.nextFloat(14.0f, 28.0f);
            p.initialSize = p.size;
            p.life = rng.nextFloat(1.0f, 2.0f);
            p.maxLife = p.life;
            p.type = ParticleType::ExplosionSmoke;
            emit(p);
        }
    }

    void emitTireSkid(const Vec2& pos, const Vec2& vel, FastRandom& rng) {
        Particle p;
        p.position = pos;
        p.velocity = vel * 0.05f;
        p.color = ColorRGBA(20, 20, 20, 180);
        p.size = 6.0f;
        p.initialSize = 6.0f;
        p.life = 4.0f;
        p.maxLife = 4.0f;
        p.type = ParticleType::TireSkid;
        emit(p);

        // Tire smoke puff
        if (rng.nextBool(0.35f)) {
            Particle s;
            s.position = pos;
            s.velocity = rng.nextDirection() * rng.nextFloat(10.0f, 25.0f);
            s.color = ColorRGBA(220, 220, 220, 140);
            s.size = rng.nextFloat(6.0f, 12.0f);
            s.initialSize = s.size;
            s.life = rng.nextFloat(0.3f, 0.7f);
            s.maxLife = s.life;
            s.type = ParticleType::TireSmoke;
            emit(s);
        }
    }

    void emitFlamethrower(const Vec2& pos, const Vec2& dir, FastRandom& rng) {
        for (int i = 0; i < 3; ++i) {
            Particle p;
            p.position = pos;
            float spread = rng.nextFloat(-0.25f, 0.25f);
            p.velocity = dir.rotated(spread) * rng.nextFloat(260.0f, 380.0f);
            p.color = ColorRGBA(255, rng.nextInt(60, 200), 10, 240);
            p.size = rng.nextFloat(8.0f, 16.0f);
            p.initialSize = p.size;
            p.life = rng.nextFloat(0.35f, 0.65f);
            p.maxLife = p.life;
            p.type = ParticleType::FlamethrowerFlame;
            emit(p);
        }
    }

    [[nodiscard]] const std::vector<Particle>& getParticles() const noexcept { return m_particles; }

    void clear() { m_particles.clear(); }

private:
    std::vector<Particle> m_particles;
};

} // namespace gta2
