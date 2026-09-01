# Tests CMakeLists.txt
set(TEST_SOURCES
    test_math.cpp
    test_physics.cpp
    test_gangs.cpp
    test_wanted.cpp
    test_weapons.cpp
    test_missions.cpp
    test_headless_game.cpp
)

add_executable(gta2_tests
    ${TEST_SOURCES}
)

target_link_libraries(gta2_tests PRIVATE
    gta2_core
    Catch2::Catch2WithMain
)

target_include_directories(gta2_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

include(CTest)
include(Catch)
catch_discover_tests(gta2_tests)
