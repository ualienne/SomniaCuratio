#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace somnia {

class SceneManager;

class Game {
 public:
  Game();

  ~Game();

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  int run();

 private:
  void processEvents();
  void update(float deltaTime);
  void render();

  sf::RenderWindow m_window;

  std::unique_ptr<SceneManager> m_sceneManager;

  static constexpr unsigned int kTargetFps = 60;
};

}  // namespace somnia
