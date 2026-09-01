#pragma once

#include <cstdint>
#include <random>
#include "Math.hpp"

namespace gta2 {

class FastRandom {
public:
    explicit FastRandom(uint64_t seed = 0x853c49e6748fea9bULL) {
        setSeed(seed);
    }

    void setSeed(uint64_t seed) noexcept {
        s[0] = seed ^ 0x5555555555555555ULL;
        s[1] = (seed + 0x9e3779b97f4a7c15ULL) ^ 0xAAAAAAAAAAAAAAAAULL;
        if (s[0] == 0 && s[1] == 0) {
            s[0] = 1;
        }
    }

    uint64_t nextU64() noexcept {
        uint64_t s1 = s[0];
        const uint64_t s0 = s[1];
        s[0] = s0;
        s1 ^= s1 << 23;
        s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
        return s[1] + s0;
    }

    int nextInt(int min, int max) noexcept {
        if (min >= max) return min;
        uint64_t r = nextU64();
        return min + static_cast<int>(r % static_cast<uint64_t>(max - min + 1));
    }

    float nextFloat(float min = 0.0f, float max = 1.0f) noexcept {
        if (min >= max) return min;
        constexpr float factor = 1.0f / static_cast<float>(0xFFFFFF);
        uint32_t r = static_cast<uint32_t>(nextU64() & 0xFFFFFF);
        return min + (static_cast<float>(r) * factor) * (max - min);
    }

    bool nextBool(float probability = 0.5f) noexcept {
        return nextFloat(0.0f, 1.0f) < probability;
    }

    Vec2 nextVec2(float minX, float maxX, float minY, float maxY) noexcept {
        return {nextFloat(minX, maxX), nextFloat(minY, maxY)};
    }

    Vec2 nextDirection() noexcept {
        float ang = nextFloat(0.0f, TWO_PI);
        return Vec2::fromAngle(ang);
    }

private:
    uint64_t s[2] = {0, 0};
};

// Global static thread-safe or engine random instance
inline FastRandom& globalRandom() {
    static thread_local FastRandom rng(1337);
    return rng;
}

} // namespace gta2
