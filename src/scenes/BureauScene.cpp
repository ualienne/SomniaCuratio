#include "scenes/BureauScene.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <iostream>

#include "scenes/ArachnophobiaScene.h"
#include "ui/DialogueManager.h"

namespace somnia {

BureauScene::BureauScene(sf::RenderWindow& window,
                         const std::string& spawnPointName,
                         SceneManager& sceneManager)
    : m_window(window),
      m_sceneManager(sceneManager),
      m_camera({static_cast<float>(window.getSize().x),
                static_cast<float>(window.getSize().y)}),
      m_spawnPointName(spawnPointName) {
  if (m_font.openFromFile("assets/fonts/PressStart2P.ttf")) {
    m_fontLoaded = true;
    m_helpText = std::make_unique<sf::Text>(
        m_font, "WASD - move   E - interact   ESC - quit", 12u);
    m_helpText->setFillColor(sf::Color(220, 220, 240, 200));
  } else {
    std::cerr << "[BureauScene] font missing\n";
  }

  if (m_map.loadFromFile("assets/maps/bureau.tmx")) {
    m_mapReady = true;
    m_camera.setWorldSize(m_map.mapSizePixels());
    m_player.setTileSize(m_map.tileSizePixels());
  } else {
    std::cerr << "[BureauScene] FAILED to load bureau.tmx\n";
  }

  struct NpcDef {
    std::string spawn, texture, name, greeting;
  };
  const std::vector<NpcDef> npcDefs = {
      {"npc_voss", "assets/sprites/characters/voss.png", "Dr. Emil Voss",
       "Ты как бледная поганка. Кошмарики задрали?"},
      {"npc_kira", "assets/sprites/characters/kira.png", "Kira", "Клац-клац.."},
      {"npc_hayes", "assets/sprites/characters/hayes.png", "Hayes",
       "Не трогай мои файлы."},
  };
  for (const auto& def : npcDefs) {
    if (const auto* sp = m_map.findSpawn(def.spawn)) {
      auto npc = std::make_unique<NPC>();
      if (npc->loadTexture(def.texture)) {
        npc->placeAt(sp->position);
        npc->setScale(4.f);
        npc->setName(def.name);
        npc->setGreeting(def.greeting);
        m_npcs.push_back(std::move(npc));
      }
    }
  }

  if (!m_player.loadTexture("assets/sprites/characters/agent.png"))
    std::cerr << "[BureauScene] agent.png not loaded\n";

  if (m_mapReady) {
    if (const auto* sp = m_map.findSpawn(m_spawnPointName)) {
      const auto ts = m_map.tileSizePixels();
      m_player.placeOnTile({static_cast<int>(sp->position.x / ts.x),
                            static_cast<int>(sp->position.y / ts.y)},
                           Facing::Down);
    } else {
      std::cerr << "[BureauScene] spawn '" << m_spawnPointName
                << "' not found\n";
      m_player.placeOnTile({1, 1}, Facing::Down);
    }
    m_camera.centerOn(m_player.worldCenter());
  }
}

void BureauScene::onResume() {
  if (!m_pendingMessage.empty()) {
    DialogueManager::instance().showSingleReplica("Система", m_pendingMessage);
    m_pendingMessage.clear();
  }
}

void BureauScene::handleEvent(const sf::Event& event) {
  if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
    if (key->code == sf::Keyboard::Key::Escape) {
      m_window.close();
      return;
    }
    using Key = sf::Keyboard::Key;
    if (key->code == Key::D || key->code == Key::Right) {
      m_nextDirection = Facing::Right;
    } else if (key->code == Key::A || key->code == Key::Left) {
      m_nextDirection = Facing::Left;
    } else if (key->code == Key::S || key->code == Key::Down) {
      m_nextDirection = Facing::Down;
    } else if (key->code == Key::W || key->code == Key::Up) {
      m_nextDirection = Facing::Up;
    }

    if (key->code == sf::Keyboard::Key::E) {
      auto& dlg = DialogueManager::instance();

      if (dlg.isVisible()) {
        dlg.hide();
        return;
      }

      const auto playerTile = m_player.currentTile();
      const auto ts = m_map.tileSizePixels();

      // --- VR-шлем: ищем сначала в interactables, потом в spawns ----------
      auto tryFindVR = [&]() -> const MapObject* {
        for (const auto& obj : m_map.interactables())
          if (obj.name == "vr_helmet") return &obj;
        if (const auto* sp = m_map.findSpawn("vr_helmet")) return sp;
        return nullptr;
      };

      if (const auto* vr = tryFindVR()) {
        const int vrX = static_cast<int>(vr->position.x / ts.x);
        const int vrY = static_cast<int>(vr->position.y / ts.y);
        std::cout << "[DEBUG] VR tile=(" << vrX << "," << vrY << ") player=("
                  << playerTile.x << "," << playerTile.y << ")\n";

        if (std::abs(playerTile.x - vrX) <= 3 &&
            std::abs(playerTile.y - vrY) <= 7) {
          m_sceneManager.pushScene(std::make_unique<ArachnophobiaScene>(
              m_window, "player_spawn_default",
              [&mgr = m_sceneManager, this](NightmareOutcome outcome) {
                switch (outcome) {
                  case NightmareOutcome::Therapy:
                    m_pendingMessage = "Кошмар побеждён.\nСтрах отступил.";
                    break;
                  case NightmareOutcome::Failure:
                    m_pendingMessage =
                        "Кошмар поглотил тебя...\nНо ты проснулась.";
                    break;
                  case NightmareOutcome::Neutral:
                    m_pendingMessage = "Ты сбежала из кошмара.\nСтрах остался.";
                    break;
                }
                mgr.popScene();
              }));
          return;
        }
      } else {
        std::cerr << "[BureauScene] vr_helmet not found on map!\n";
      }

      // --- NPC диалог ------------------------------------------------------
      for (const auto& npc : m_npcs) {
        const sf::Vector2f pos = npc->position();
        const int npcX = static_cast<int>(pos.x / ts.x);
        const int npcY = static_cast<int>(pos.y / ts.y);
        if (std::abs(playerTile.x - npcX) <= 3 &&
            std::abs(playerTile.y - npcY) <= 3) {
          dlg.showSingleReplica(npc->name(), npc->greeting());
          break;
        }
      }
    }
  }
}

void BureauScene::update(float deltaTime) {
  if (!m_mapReady) return;

  if (DialogueManager::instance().isVisible()) {
    m_nextDirection.reset();
  } else {
    if (!m_player.isMoving() && m_nextDirection.has_value()) {
      m_player.tryStartStep(m_nextDirection.value(), m_map);
      m_nextDirection.reset();
    }
    if (m_player.isMoving()) {
      m_nextDirection.reset();
    }
  }

  m_player.update(deltaTime);
  m_camera.centerOn(m_player.worldCenter());
}

void BureauScene::render(sf::RenderWindow& window) {
  window.clear(sf::Color(18, 22, 40));
  if (!m_mapReady) return;

  window.setView(m_camera.view());
  m_map.drawBackground(window);
  for (const auto& npc : m_npcs) window.draw(*npc);
  window.draw(m_player);
  m_map.drawDecorAbove(window);

  window.setView(window.getDefaultView());
  if (m_fontLoaded && m_helpText) {
    m_helpText->setPosition(
        {8.f, static_cast<float>(window.getSize().y) - 24.f});
    window.draw(*m_helpText);
  }

  const sf::View& cv = m_camera.view();
  const float playerScreenY =
      (m_player.worldCenter().y - cv.getCenter().y + cv.getSize().y / 2.f) *
      (static_cast<float>(window.getSize().y) / cv.getSize().y);
  DialogueManager::instance().draw(window, m_font, playerScreenY);
}

}  // namespace somnia
