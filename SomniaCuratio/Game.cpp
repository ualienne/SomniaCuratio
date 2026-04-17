#include "Game.h"

Game::Game() : m_window(sf::VideoMode({800, 600}), "Somnia Curatio") {
  m_background.load("Bureau_map.png", m_window.getSize());
  m_player.load(
      "Alex_idle_16x16.png");  // Я могла случайно не тот спрайт подрубить
}  // Страница, фон, игрок

void Game::run() {
  while (m_window.isOpen()) {
    processEvents();
    render();
  }
}

void Game::processEvents() {
  while (const std::optional event = m_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      m_window.close();
    }

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      m_player.handleInput(keyPressed->code);
    }
  }
}

void Game::render() {
  m_window.clear();
  m_background.draw(m_window);
  m_player.draw(m_window);
  m_window.display();
}  // Игравой экран
