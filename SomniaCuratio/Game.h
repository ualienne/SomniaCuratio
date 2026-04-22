#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>

#include "Background.h"
#include "CombatManager.h"
#include "CombatUI.h"
#include "DialogueManager.h"
#include "Inventory.h"
#include "Npc.h"
#include "Player.h"

class Game {
 public:
  Game();
  void run();

 private:
  void processEvents();
  void render();

  sf::RenderWindow m_window;
  Background m_background;
  Player m_player;
  Inventory m_inventory;
  sf::Font m_font;
  std::vector<std::unique_ptr<Npc>> m_npcs;
  CombatManager m_combat;
  bool m_inCombat = false;
  CombatUI m_combatUI;
};

#endif
