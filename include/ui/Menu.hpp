#pragma once

#include <string>
#include <vector>
#include "../core/Types.hpp"
#include "../core/Math.hpp"

namespace gta2 {

struct MenuItem {
    std::string text;
    int actionId = 0;
    bool isSelected = false;
};

class Menu {
public:
    Menu();

    void initializeTitleMenu();
    void initializePauseMenu();
    void initializeGameOverMenu();

    void selectNext();
    void selectPrevious();
    [[nodiscard]] int getSelectedAction() const;

    [[nodiscard]] const std::vector<MenuItem>& getItems() const noexcept { return m_items; }
    [[nodiscard]] size_t getSelectedIndex() const noexcept { return m_selectedIndex; }
    [[nodiscard]] const std::string& getTitle() const noexcept { return m_title; }

    void setTitle(const std::string& title) { m_title = title; }

private:
    std::string m_title;
    std::vector<MenuItem> m_items;
    size_t m_selectedIndex = 0;
};

} // namespace gta2
