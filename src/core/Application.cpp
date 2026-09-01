#include "core/Application.hpp"
#include <iostream>
#include <cmath>

#ifndef HEADLESS_MODE
#include "raylib.h"
#endif

namespace gta2 {

Application::Application() = default;

Application::~Application() {
    shutdown();
}

bool Application::initialize(int width, int height, bool fullscreen) {
    m_windowWidth = width;
    m_windowHeight = height;

#ifndef HEADLESS_MODE
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(m_windowWidth, m_windowHeight, globalConfig().windowTitle.c_str());
    SetTargetFPS(globalConfig().targetFPS);
#endif

    m_engine.initialize(false);
    m_titleMenu.initializeTitleMenu();
    m_pauseMenu.initializePauseMenu();
    m_gameOverMenu.initializeGameOverMenu();

    m_running = true;
    m_state = GameState::TitleScreen;
    return true;
}

void Application::shutdown() {
#ifndef HEADLESS_MODE
    if (m_running) {
        CloseWindow();
    }
#endif
    m_running = false;
}

void Application::run() {
#ifndef HEADLESS_MODE
    while (m_running && !WindowShouldClose()) {
        float dt = GetFrameTime();
        dt = std::min(dt, 0.1f); // Cap delta time to prevent spiral of death

        processInput(dt);
        update(dt);
        render();
    }
#endif
}

void Application::runHeadlessTicks(uint32_t ticks, float dt) {
    m_engine.initialize(true);
    for (uint32_t i = 0; i < ticks; ++i) {
        m_engine.update(dt);
    }
}

void Application::processInput(float dt) {
#ifndef HEADLESS_MODE
    if (m_state == GameState::TitleScreen) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) m_titleMenu.selectPrevious();
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) m_titleMenu.selectNext();
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            int action = m_titleMenu.getSelectedAction();
            if (action == 1) { // Start Game
                m_state = GameState::Playing;
            } else if (action == 4) { // Exit
                m_running = false;
            }
        }
        return;
    }

    if (m_state == GameState::Paused) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) m_pauseMenu.selectPrevious();
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) m_pauseMenu.selectNext();
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            int action = m_pauseMenu.getSelectedAction();
            if (action == 1) m_state = GameState::Playing;
            else if (action == 2) { m_engine.resetGame(); m_state = GameState::Playing; }
            else if (action == 3) m_state = GameState::TitleScreen;
        }
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (m_state == GameState::Playing) m_state = GameState::Paused;
        else if (m_state == GameState::Paused) m_state = GameState::Playing;
    }

    if (m_state == GameState::Playing) {
        Vec2 moveInput = {0.0f, 0.0f};
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) moveInput.y += 1.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) moveInput.y -= 1.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) moveInput.x -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveInput.x += 1.0f;

        bool attack = IsKeyDown(KEY_LEFT_CONTROL) || IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        bool actionEnterExit = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_F);
        bool handbrake = IsKeyDown(KEY_SPACE);
        bool specialAction = IsKeyPressed(KEY_LEFT_SHIFT);
        bool nextWeapon = IsKeyPressed(KEY_E) || (GetMouseWheelMove() > 0.0f);
        bool prevWeapon = IsKeyPressed(KEY_Q) || (GetMouseWheelMove() < 0.0f);
        bool nextRadio = IsKeyPressed(KEY_R);

        m_engine.handlePlayerInput(moveInput, attack, actionEnterExit, handbrake, specialAction, nextWeapon, prevWeapon, nextRadio, dt);
    }
#endif
}

void Application::update(float dt) {
    if (m_state == GameState::Playing) {
        m_engine.update(dt);
        if (m_engine.getPlayer().isDead()) {
            m_state = GameState::Wasted;
        }
    }
}

void Application::render() {
#ifndef HEADLESS_MODE
    BeginDrawing();
    ClearBackground(BLACK);

    if (m_state == GameState::Playing || m_state == GameState::Paused || m_state == GameState::Wasted) {
        renderGameWorld();
        renderLightingAndShadows();
        renderHUD();
    }

    if (m_state != GameState::Playing) {
        renderMenus();
    }

    EndDrawing();
#endif
}

void Application::renderGameWorld() {
#ifndef HEADLESS_MODE
    const auto& cam = m_engine.getCamera();
    const auto& map = m_engine.getMap();
    float tileSize = map.getTileSize();
    Rect2D visibleBounds = cam.getVisibleWorldBounds(m_windowWidth, m_windowHeight);

    int startX = std::max(0, static_cast<int>(visibleBounds.x / tileSize));
    int endX = std::min(map.getWidth() - 1, static_cast<int>((visibleBounds.x + visibleBounds.width) / tileSize) + 1);
    int startY = std::max(0, static_cast<int>(visibleBounds.y / tileSize));
    int endY = std::min(map.getHeight() - 1, static_cast<int>((visibleBounds.y + visibleBounds.height) / tileSize) + 1);

    // 1. Draw Map Tiles
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            Tile t = map.getTile(x, y);
            Vec2 worldPos = {static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize};
            Vec2 screenPos = cam.worldToScreen(worldPos, m_windowWidth, m_windowHeight);
            float scaledSize = tileSize * cam.getZoom();

            Color c = {t.tint.r, t.tint.g, t.tint.b, t.tint.a};
            DrawRectangle(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), static_cast<int>(scaledSize) + 1, static_cast<int>(scaledSize) + 1, c);
        }
    }

    // 2. Draw Particles (Tire skids, blood)
    for (const auto& p : m_engine.getParticles().getParticles()) {
        Vec2 sp = cam.worldToScreen(p.position, m_windowWidth, m_windowHeight);
        Color c = {p.color.r, p.color.g, p.color.b, p.color.a};
        DrawCircle(static_cast<int>(sp.x), static_cast<int>(sp.y), p.size * cam.getZoom(), c);
    }

    // 3. Draw Vehicles
    for (const auto& vPtr : m_engine.getVehicles()) {
        const auto& v = *vPtr;
        if (!v.isActive()) continue;

        Vec2 sp = cam.worldToScreen(v.getPosition(), m_windowWidth, m_windowHeight);
        float w = v.getSpecs().width * cam.getZoom();
        float h = v.getSpecs().length * cam.getZoom();

        Rectangle rec = {sp.x, sp.y, w, h};
        Vector2 origin = {w * 0.5f, h * 0.5f};
        float angleDeg = (v.getRotation() + PI * 0.5f) * RAD_TO_DEG;

        Color bodyColor = {v.getPrimaryColor().r, v.getPrimaryColor().g, v.getPrimaryColor().b, 255};
        DrawRectanglePro(rec, origin, angleDeg, bodyColor);

        // Headlights cone at night
        if (m_engine.getDayNightCycle().areStreetlightsActive() || v.isSirenActive()) {
            Vec2 fwd = v.getForwardVector();
            Vec2 lightPos = sp + Vec2{fwd.x * 20.0f, fwd.y * 20.0f};
            DrawCircleGradient(static_cast<int>(lightPos.x), static_cast<int>(lightPos.y), 45.0f * cam.getZoom(), ColorAlpha(YELLOW, 0.4f), ColorAlpha(YELLOW, 0.0f));
        }
    }

    // 4. Draw Pedestrians
    for (const auto& pPtr : m_engine.getPedestrians()) {
        const auto& p = *pPtr;
        if (!p.isActive() || p.isInVehicle()) continue;

        Vec2 sp = cam.worldToScreen(p.getPosition(), m_windowWidth, m_windowHeight);
        float r = p.getRadius() * cam.getZoom();
        Color shirt = {p.getShirtColor().r, p.getShirtColor().g, p.getShirtColor().b, 255};
        DrawCircle(static_cast<int>(sp.x), static_cast<int>(sp.y), r, shirt);
    }

    // 5. Draw Player (if on foot)
    const auto& player = m_engine.getPlayer();
    if (!player.isInVehicle() && !player.isDead()) {
        Vec2 sp = cam.worldToScreen(player.getPosition(), m_windowWidth, m_windowHeight);
        float r = player.getRadius() * cam.getZoom();
        DrawCircle(static_cast<int>(sp.x), static_cast<int>(sp.y), r, GREEN);
        // Direction indicator
        Vec2 fwd = Vec2::fromAngle(player.getRotation()) * (r * 1.5f);
        DrawLine(static_cast<int>(sp.x), static_cast<int>(sp.y), static_cast<int>(sp.x + fwd.x), static_cast<int>(sp.y + fwd.y), WHITE);
    }

    // 6. Draw Projectiles
    for (const auto& projPtr : m_engine.getProjectiles()) {
        const auto& proj = *projPtr;
        if (!proj.isActive()) continue;
        Vec2 sp = cam.worldToScreen(proj.getPosition(), m_windowWidth, m_windowHeight);
        DrawCircle(static_cast<int>(sp.x), static_cast<int>(sp.y), proj.getRadius() * cam.getZoom(), YELLOW);
    }
#endif
}

void Application::renderLightingAndShadows() {
#ifndef HEADLESS_MODE
    float darkness = m_engine.getDayNightCycle().getDarkness();
    if (darkness > 0.05f) {
        DrawRectangle(0, 0, m_windowWidth, m_windowHeight, ColorAlpha(BLACK, darkness * 0.65f));
    }
#endif
}

void Application::renderHUD() {
#ifndef HEADLESS_MODE
    const auto& hud = m_engine.getHUD();

    // 1. Cash Counter
    std::string cashStr = "$" + std::to_string(hud.getCash());
    DrawText(cashStr.c_str(), m_windowWidth - 220, 20, 32, GREEN);

    // Multiplier
    std::string multStr = "x" + std::to_string(hud.getMultiplier());
    DrawText(multStr.c_str(), m_windowWidth - 70, 26, 22, YELLOW);

    // 2. Health & Armor Bars
    DrawRectangle(m_windowWidth - 220, 60, 180, 16, DARKGRAY);
    DrawRectangle(m_windowWidth - 220, 60, static_cast<int>(180.0f * (hud.getHealth() / 100.0f)), 16, RED);
    DrawRectangleLines(m_windowWidth - 220, 60, 180, 16, WHITE);

    DrawRectangle(m_windowWidth - 220, 82, 180, 12, DARKGRAY);
    DrawRectangle(m_windowWidth - 220, 82, static_cast<int>(180.0f * (hud.getArmor() / 100.0f)), 12, BLUE);
    DrawRectangleLines(m_windowWidth - 220, 82, 180, 12, WHITE);

    // 3. Wanted Stars
    for (int i = 0; i < 6; ++i) {
        Color starColor = (i < hud.getWantedStars()) ? GOLD : DARKGRAY;
        DrawText("*", m_windowWidth - 220 + (i * 30), 100, 32, starColor);
    }

    // 4. Weapon & Ammo
    std::string wepStr = hud.getWeaponName() + " [" + std::to_string(hud.getAmmo()) + "]";
    DrawText(wepStr.c_str(), m_windowWidth - 220, 140, 20, RAYWHITE);

    // 5. Radio Station
    DrawText(hud.getRadioStationName().c_str(), 20, m_windowHeight - 40, 20, SKYBLUE);

    // 6. Respect Meters
    DrawText("ZAIBATSU:", 20, 20, 16, RED);
    DrawRectangle(110, 22, 100, 12, DARKGRAY);
    DrawRectangle(110, 22, static_cast<int>(std::clamp((hud.getZaibatsuRespect() + 100.0f) * 0.5f, 0.0f, 100.0f)), 12, RED);

    DrawText("LOONIES:", 20, 40, 16, GREEN);
    DrawRectangle(110, 42, 100, 12, DARKGRAY);
    DrawRectangle(110, 42, static_cast<int>(std::clamp((hud.getLooniesRespect() + 100.0f) * 0.5f, 0.0f, 100.0f)), 12, GREEN);

    DrawText("YAKUZA:", 20, 60, 16, BLUE);
    DrawRectangle(110, 62, 100, 12, DARKGRAY);
    DrawRectangle(110, 62, static_cast<int>(std::clamp((hud.getYakuzaRespect() + 100.0f) * 0.5f, 0.0f, 100.0f)), 12, BLUE);

    // 7. Mission Banner
    if (!hud.getMissionTitle().empty()) {
        DrawRectangle(m_windowWidth / 2 - 200, 15, 400, 45, ColorAlpha(BLACK, 0.8f));
        DrawRectangleLines(m_windowWidth / 2 - 200, 15, 400, 45, GOLD);
        DrawText(hud.getMissionTitle().c_str(), m_windowWidth / 2 - 180, 20, 20, GOLD);
        DrawText(hud.getObjectiveText().c_str(), m_windowWidth / 2 - 180, 40, 15, WHITE);
    }

    // 8. Floating Texts
    for (const auto& msg : hud.getFloatingText().getMessages()) {
        Vec2 sp = m_engine.getCamera().worldToScreen(msg.worldPosition, m_windowWidth, m_windowHeight);
        Color c = {msg.color.r, msg.color.g, msg.color.b, msg.color.a};
        DrawText(msg.text.c_str(), static_cast<int>(sp.x), static_cast<int>(sp.y), static_cast<int>(20.0f * msg.scale), c);
    }
#endif
}

void Application::renderMenus() {
#ifndef HEADLESS_MODE
    if (m_state == GameState::TitleScreen) {
        DrawRectangle(0, 0, m_windowWidth, m_windowHeight, ColorAlpha(BLACK, 0.85f));
        DrawText("GTA 2: CITY OF CHAOS", m_windowWidth / 2 - 260, 120, 42, GOLD);
        DrawText("TOP-DOWN OPEN WORLD ACTION", m_windowWidth / 2 - 180, 175, 20, LIGHTGRAY);

        int startY = 280;
        for (size_t i = 0; i < m_titleMenu.getItems().size(); ++i) {
            const auto& item = m_titleMenu.getItems()[i];
            Color col = item.isSelected ? YELLOW : RAYWHITE;
            std::string prefix = item.isSelected ? "> " : "  ";
            DrawText((prefix + item.text).c_str(), m_windowWidth / 2 - 140, startY + static_cast<int>(i * 45), 24, col);
        }
    } else if (m_state == GameState::Paused) {
        DrawRectangle(0, 0, m_windowWidth, m_windowHeight, ColorAlpha(BLACK, 0.7f));
        DrawText("PAUSED", m_windowWidth / 2 - 80, 180, 40, WHITE);
        int startY = 280;
        for (size_t i = 0; i < m_pauseMenu.getItems().size(); ++i) {
            const auto& item = m_pauseMenu.getItems()[i];
            Color col = item.isSelected ? YELLOW : RAYWHITE;
            std::string prefix = item.isSelected ? "> " : "  ";
            DrawText((prefix + item.text).c_str(), m_windowWidth / 2 - 120, startY + static_cast<int>(i * 45), 24, col);
        }
    } else if (m_state == GameState::Wasted) {
        DrawRectangle(0, 0, m_windowWidth, m_windowHeight, ColorAlpha(MAROON, 0.8f));
        DrawText("WASTED / BUSTED", m_windowWidth / 2 - 200, 220, 48, RED);
        DrawText("Press ENTER to Respawn", m_windowWidth / 2 - 140, 320, 22, WHITE);
        if (IsKeyPressed(KEY_ENTER)) {
            m_engine.resetGame();
            m_state = GameState::Playing;
        }
    }
#endif
}

} // namespace gta2
