#include "core/Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>

#include "core/SceneManager.h"
#include "scenes/BureauScene.h"

namespace somnia {

Game::Game()
    : m_window(sf::VideoMode({1600u, 900u}), "Somnia Curatio",
               sf::Style::Titlebar | sf::Style::Close),
      m_sceneManager(std::make_unique<SceneManager>()) {
  m_window.setFramerateLimit(kTargetFps);
  m_window.setKeyRepeatEnabled(false);

  m_sceneManager->pushScene(std::make_unique<BureauScene>(
      m_window, "player_spawn_default", *m_sceneManager));
}

Game::~Game() = default;

int Game::run() {
  sf::Clock clock;

  while (m_window.isOpen()) {
    const float dt = clock.restart().asSeconds();
    const float cappedDt = (dt > 1.0f / 30.0f) ? (1.0f / 30.0f) : dt;

    processEvents();
    update(cappedDt);
    render();

    if (m_sceneManager->empty()) {
      m_window.close();
    }
  }

  return 0;
}

void Game::processEvents() {
  while (const std::optional<sf::Event> event = m_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      m_window.close();
      return;
    }
    m_sceneManager->handleEvent(*event);
  }
}

void Game::update(float deltaTime) { m_sceneManager->update(deltaTime); }

void Game::render() {
  m_window.clear(sf::Color(12, 16, 32));
  m_sceneManager->render(m_window);
  m_window.display();
}

}
