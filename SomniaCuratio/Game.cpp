#include "Game.h"
#include <cmath>
#include <iostream>

Game::Game() : m_window(sf::VideoMode({800, 600}), "SomniaCuratio") {
  if (!m_font.openFromFile("arial.ttf")) {
  }

  m_background.load("Bureau_map.png", m_window.getSize());
  m_player.load("Adam_idle_16x16.png");


  // Амелия
  auto Amelia = std::make_unique<Npc>(u8"Амелия", "Amelia_idle_16x16.png");
  if (Amelia->load()) {
    Amelia->setPosition({200.f, 250.f});
    Amelia->addReplica(u8"Привет! Как тебе наша библиотека?");
    Amelia->addReplica(u8"Говорят, в архивах спрятано что-то ценное...");
    Amelia->addReplica(u8"Я просто люблю читать.");
    m_npcs.push_back(std::move(Amelia));
  }

  // Боб
  auto Bob = std::make_unique<Npc>(u8"Боб", "Bob_idle_16x16.png");
  if (Bob->load()) {
    Bob->setPosition({500.f, 100.f});
    Bob->addReplica(u8"Я Боб. Я здесь за главного.");
    Bob->addReplica(u8"Не шуми в коридорах.");
    m_npcs.push_back(std::move(Bob));
  }

  // Алекс
  auto Alex = std::make_unique<Npc>(u8"Алекс", "Alex_idle_16x16.png");
  if (Alex->load()) {
    Alex->setPosition({100.f, 450.f});
    Alex->addReplica(u8"Тсс... Ты слышишь этот звук?");
    Alex->addReplica(u8"Я ищу выход отсюда.");
    m_npcs.push_back(std::move(Alex));
  }
}

void Game::run() {
  sf::Clock clock;
  while (m_window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();
    processEvents();
    render();
  }
}

void Game::processEvents() {
  while (const std::optional event = m_window.pollEvent()) {
    // Закрытие окна (Крестик)
    if (event->is<sf::Event::Closed>()) {
      m_window.close();
    }

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::E) {
        if (DialogueManager::instance().isVisible()) {
          DialogueManager::instance().hide();
        } else {
          sf::Vector2f playerPos = m_player.getPosition();
          const float maxDistance = 50.0f;
          for (auto& npc : m_npcs) {
            sf::Vector2f npcPos = npc->getPosition();
            float dx = playerPos.x - npcPos.x;
            float dy = playerPos.y - npcPos.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= maxDistance) {
              std::string phrase = npc->getRandomReplica();
              DialogueManager::instance().showSingleReplica(npc->getName(),
                                                            phrase);
              break;
            }
          }
        }
      }
      if (keyPressed->code == sf::Keyboard::Key::Q) {
        m_inventory.toggle();
      }
      if (!DialogueManager::instance().isVisible()) {
        m_player.handleInput(keyPressed->code);
      }
    }
  }
}

void Game::render() {
  m_window.clear();
  m_background.draw(m_window);
  for (auto& npc : m_npcs) {
    npc->draw(m_window);
  }
  m_player.draw(m_window);
  m_inventory.draw(m_window, m_font);
  DialogueManager::instance().draw(m_window, m_font);
  m_window.display();
}
