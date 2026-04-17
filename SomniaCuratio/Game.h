#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <optional>

#include "Background.h"
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
};

#endif
