# ==============================================================================
# GTA 2: City of Chaos - Modern C++20 Open-World Game Engine
# ==============================================================================
cmake_minimum_required(VERSION 3.22)
project(GTA2CityOfChaos
    VERSION 1.0.0
    DESCRIPTION "Advanced C++20 GTA 2 Clone Open-World Game Engine"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option Flags
option(BUILD_TESTS "Build automated test suite" ON)
option(BUILD_HEADLESS_ONLY "Build in headless mode without window creation" OFF)

if(MSVC)
    add_compile_options(/W3 /permissive- /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES NOMINMAX)
else()
    add_compile_options(-Wall -Wextra -Wno-unused-parameter)
endif()

# ------------------------------------------------------------------------------
# Dependencies via FetchContent (Autonomous CI Build Support)
# ------------------------------------------------------------------------------
include(FetchContent)
set(FETCHCONTENT_QUIET ON)

# 1. Raylib (Windowing, Audio, Rendering, Input)
if(NOT BUILD_HEADLESS_ONLY)
    message(STATUS "Fetching Raylib 5.0...")
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
        GIT_SHALLOW TRUE
    )
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(raylib)
endif()

# 2. Catch2 (Automated Testing Framework)
if(BUILD_TESTS)
    message(STATUS "Fetching Catch2 v3.5.2...")
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)
endif()

# ------------------------------------------------------------------------------
# Core Engine Library Target
# ------------------------------------------------------------------------------
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(ENGINE_SOURCES
    src/core/Application.cpp
    src/core/Config.cpp
    src/core/Engine.cpp
    src/core/Math.cpp
    src/core/Random.cpp
    src/world/CityMap.cpp
    src/world/RoadGraph.cpp
    src/world/Camera.cpp
    src/world/DayNightCycle.cpp
    src/entities/Entity.cpp
    src/entities/Player.cpp
    src/entities/Pedestrian.cpp
    src/entities/PedestrianAI.cpp
    src/entities/Vehicle.cpp
    src/entities/Projectile.cpp
    src/entities/ParticleSystem.cpp
    src/systems/PhysicsSystem.cpp
    src/systems/GangSystem.cpp
    src/systems/WantedSystem.cpp
    src/systems/MissionSystem.cpp
    src/systems/TrafficSystem.cpp
    src/systems/WeaponSystem.cpp
    src/systems/RadioSystem.cpp
    src/systems/AudioSystem.cpp
    src/ui/HUD.cpp
    src/ui/Minimap.cpp
    src/ui/Menu.cpp
    src/ui/FloatingText.cpp
    src/audio/ProceduralAudio.cpp
)

set(ENGINE_HEADERS
    include/core/Application.hpp
    include/core/Config.hpp
    include/core/Engine.hpp
    include/core/Math.hpp
    include/core/Random.hpp
    include/core/Types.hpp
    include/world/Tile.hpp
    include/world/CityMap.hpp
    include/world/RoadGraph.hpp
    include/world/Camera.hpp
    include/world/DayNightCycle.hpp
    include/entities/Entity.hpp
    include/entities/Player.hpp
    include/entities/Pedestrian.hpp
    include/entities/PedestrianAI.hpp
    include/entities/Vehicle.hpp
    include/entities/VehicleTypes.hpp
    include/entities/Projectile.hpp
    include/entities/ParticleSystem.hpp
    include/systems/PhysicsSystem.hpp
    include/systems/GangSystem.hpp
    include/systems/WantedSystem.hpp
    include/systems/MissionSystem.hpp
    include/systems/TrafficSystem.hpp
    include/systems/WeaponSystem.hpp
    include/systems/RadioSystem.hpp
    include/systems/AudioSystem.hpp
    include/ui/HUD.hpp
    include/ui/Minimap.hpp
    include/ui/Menu.hpp
    include/ui/FloatingText.hpp
    include/audio/ProceduralAudio.hpp
)

add_library(gta2_core STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(gta2_core PUBLIC
    ${ENGINE_INCLUDE_DIR}
)

if(TARGET raylib)
    target_link_libraries(gta2_core PUBLIC raylib)
else()
    target_compile_definitions(gta2_core PUBLIC HEADLESS_MODE=1)
endif()

# ------------------------------------------------------------------------------
# Executable Target (Game Application)
# ------------------------------------------------------------------------------
if(NOT BUILD_HEADLESS_ONLY)
    add_executable(gta2_game
        src/main.cpp
    )

    target_link_libraries(gta2_game PRIVATE
        gta2_core
    )
endif()

# ------------------------------------------------------------------------------
# Automated Tests Target
# ------------------------------------------------------------------------------
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
