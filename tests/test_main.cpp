#include "testing/TestHarness.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "\n==================================================" << std::endl;
    std::cout << " GTA 2: City of Chaos - Automated CI Test Suite " << std::endl;
    std::cout << "==================================================" << std::endl;

    const auto& testCases = TestFramework::getRegisteredCases();
    std::cout << "Discovered " << testCases.size() << " test suites.\n" << std::endl;

    for (const auto& tc : testCases) {
        std::cout << "[RUNNING] " << tc.name << " (" << tc.tag << ") ... " << std::flush;
        TestFramework::g_activeSection = "";
        int prevFails = TestFramework::g_failedCount;

        try {
            tc.func();
            if (TestFramework::g_failedCount == prevFails) {
                std::cout << "SUCCESS" << std::endl;
            } else {
                std::cout << "FAILED" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "EXCEPTION: " << e.what() << std::endl;
            TestFramework::g_failedCount++;
        } catch (...) {
            std::cout << "UNKNOWN EXCEPTION" << std::endl;
            TestFramework::g_failedCount++;
        }
    }

    std::cout << "\n--------------------------------------------------" << std::endl;
    std::cout << "Results: " << TestFramework::g_passedCount << " passed, "
              << TestFramework::g_failedCount << " failed." << std::endl;
    std::cout << "==================================================\n" << std::endl;

    return (TestFramework::g_failedCount == 0) ? 0 : 1;
}
