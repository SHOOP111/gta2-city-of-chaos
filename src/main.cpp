#include "core/Application.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << " GTA 2: City of Chaos (C++20 Engine)  " << std::endl;
    std::cout << "========================================" << std::endl;

    gta2::Application app;
    if (!app.initialize(1280, 720, false)) {
        std::cerr << "Failed to initialize GTA 2 Game Application!" << std::endl;
        return 1;
    }

    app.run();
    app.shutdown();

    return 0;
}
