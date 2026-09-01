#include "ui/Menu.hpp"

namespace gta2 {

Menu::Menu() = default;

void Menu::initializeTitleMenu() {
    m_title = "GTA 2: CITY OF CHAOS";
    m_items.clear();
    m_items.push_back({"START GAME", 1, true});
    m_items.push_back({"INSTRUCTIONS & CONTROLS", 2, false});
    m_items.push_back({"CREDITS", 3, false});
    m_items.push_back({"EXIT TO DESKTOP", 4, false});
    m_selectedIndex = 0;
}

void Menu::initializePauseMenu() {
    m_title = "GAME PAUSED";
    m_items.clear();
    m_items.push_back({"RESUME GAME", 1, true});
    m_items.push_back({"RESTART DISTRICT", 2, false});
    m_items.push_back({"MAIN MENU", 3, false});
    m_selectedIndex = 0;
}

void Menu::initializeGameOverMenu() {
    m_title = "WASTED / BUSTED";
    m_items.clear();
    m_items.push_back({"RESPAWN ($5,000)", 1, true});
    m_items.push_back({"RESTART FROM SCRATCH", 2, false});
    m_items.push_back({"MAIN MENU", 3, false});
    m_selectedIndex = 0;
}

void Menu::selectNext() {
    if (m_items.empty()) return;
    m_items[m_selectedIndex].isSelected = false;
    m_selectedIndex = (m_selectedIndex + 1) % m_items.size();
    m_items[m_selectedIndex].isSelected = true;
}

void Menu::selectPrevious() {
    if (m_items.empty()) return;
    m_items[m_selectedIndex].isSelected = false;
    m_selectedIndex = (m_selectedIndex + m_items.size() - 1) % m_items.size();
    m_items[m_selectedIndex].isSelected = true;
}

int Menu::getSelectedAction() const {
    if (m_selectedIndex < m_items.size()) {
        return m_items[m_selectedIndex].actionId;
    }
    return 0;
}

} // namespace gta2
