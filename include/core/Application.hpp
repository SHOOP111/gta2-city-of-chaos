#pragma once

#include <memory>
#include "Types.hpp"
#include "Engine.hpp"
#include "../ui/Menu.hpp"

namespace gta2 {

class Application {
public:
    Application();
    ~Application();

    bool initialize(int width = 1280, int height = 720, bool fullscreen = false);
    void run();
    void runHeadlessTicks(uint32_t ticks, float dt = 1.0f / 60.0f);
    void shutdown();

    [[nodiscard]] Engine& getEngine() noexcept { return m_engine; }
    [[nodiscard]] const Engine& getEngine() const noexcept { return m_engine; }
    [[nodiscard]] GameState getState() const noexcept { return m_state; }
    void setState(GameState state) noexcept { m_state = state; }

private:
    void processInput(float dt);
    void update(float dt);
    void render();

    void renderGameWorld();
    void renderLightingAndShadows();
    void renderHUD();
    void renderMenus();

    Engine m_engine;
    GameState m_state = GameState::TitleScreen;
    Menu m_titleMenu;
    Menu m_pauseMenu;
    Menu m_gameOverMenu;

    bool m_running = false;
    int m_windowWidth = 1280;
    int m_windowHeight = 720;
    float m_accumulator = 0.0f;
};

} // namespace gta2
