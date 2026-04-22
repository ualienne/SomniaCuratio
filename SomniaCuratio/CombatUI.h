#ifndef COMBATUI_H
#define COMBATUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "CombatManager.h"

class CombatUI {
 public:
  CombatUI(sf::Font& font);
  void draw(sf::RenderWindow& window, const CombatManager& combat);

 private:
  sf::RectangleShape m_border;       // Белая рамка
  sf::RectangleShape m_bg;           // Черный фон
  sf::RectangleShape m_hpBarRed;     // HP фон
  sf::RectangleShape m_hpBarYellow;  // Текущее HP
  sf::Text m_infoText;               // Текст (имя, уровень HP)
  sf::Text m_buttons[4];             // FIGHT, ACT, ITEM, MERCY
};

#endif
