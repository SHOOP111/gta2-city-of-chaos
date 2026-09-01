#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <functional>
#include <algorithm>

namespace Catch {

struct Approx {
    double value;
    double eps = 0.001;
    double mgn = 0.001;

    explicit Approx(double v) : value(v) {}
    explicit Approx(float v) : value(static_cast<double>(v)) {}

    Approx& margin(double m) { mgn = m; return *this; }
    Approx& epsilon(double e) { eps = e; return *this; }

    [[nodiscard]] bool compare(double other) const noexcept {
        return std::abs(other - value) <= std::max(mgn, std::abs(value) * eps);
    }
};

inline bool operator==(double a, const Approx& b) noexcept { return b.compare(a); }
inline bool operator==(float a, const Approx& b) noexcept { return b.compare(static_cast<double>(a)); }
inline bool operator==(const Approx& b, double a) noexcept { return b.compare(a); }
inline bool operator==(const Approx& b, float a) noexcept { return b.compare(static_cast<double>(a)); }

} // namespace Catch

namespace TestFramework {

struct TestCase {
    std::string name;
    std::string tag;
    std::function<void()> func;
};

inline std::vector<TestCase>& getRegisteredCases() {
    static std::vector<TestCase> cases;
    return cases;
}

struct AutoRegistrar {
    AutoRegistrar(const std::string& name, const std::string& tag, std::function<void()> func) {
        getRegisteredCases().push_back({name, tag, func});
    }
};

inline int g_failedCount = 0;
inline int g_passedCount = 0;
inline std::string g_activeSection = "";

inline void recordAssert(bool result, const char* expression, const char* file, int line) {
    if (!result) {
        std::cerr << "\n  [FAILURE] " << file << ":" << line << " -> REQUIRE(" << expression << ")";
        if (!g_activeSection.empty()) {
            std::cerr << " in section: \"" << g_activeSection << "\"";
        }
        std::cerr << std::endl;
        g_failedCount++;
    } else {
        g_passedCount++;
    }
}

} // namespace TestFramework

#define COMBINE1(X, Y) X##Y
#define COMBINE(X, Y) COMBINE1(X, Y)

#define TEST_CASE(name, tag) \
    static void COMBINE(test_func_, __LINE__)(); \
    static TestFramework::AutoRegistrar COMBINE(test_reg_, __LINE__)(name, tag, COMBINE(test_func_, __LINE__)); \
    static void COMBINE(test_func_, __LINE__)()

#define SECTION(name) \
    TestFramework::g_activeSection = name;

#define REQUIRE(expr) \
    TestFramework::recordAssert(static_cast<bool>(expr), #expr, __FILE__, __LINE__)
