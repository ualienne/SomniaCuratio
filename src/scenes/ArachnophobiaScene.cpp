#include "scenes/ArachnophobiaScene.h"

#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

namespace somnia {

ArachnophobiaScene::ArachnophobiaScene(sf::RenderWindow& window,
                                       const std::string& spawnPointName,
                                       OutcomeCallback onExit)
    : m_window(window),
      m_camera({static_cast<float>(window.getSize().x),
                static_cast<float>(window.getSize().y)}),
      m_spawnPointName(spawnPointName),
      m_onExit(std::move(onExit)) {
  if (m_font.openFromFile("assets/fonts/PressStart2P.ttf"))
    m_fontLoaded = true;
  else
    std::cerr << "[ArachnophobiaScene] font missing\n";

  if (m_map.loadFromFile("assets/maps/arachnophobia.tmx")) {
    m_mapReady = true;
    m_camera.setWorldSize(m_map.mapSizePixels());
    m_player.setTileSize(m_map.tileSizePixels());
  } else {
    std::cerr << "[ArachnophobiaScene] FAILED to load arachnophobia.tmx\n";
  }

  m_player.loadTexture("assets/sprites/characters/agent.png");
  if (m_mapReady) {
    const auto ts = m_map.tileSizePixels();
    if (const auto* sp = m_map.findSpawn(m_spawnPointName)) {
      m_player.placeOnTile({static_cast<int>(sp->position.x / ts.x),
                            static_cast<int>(sp->position.y / ts.y)},
                           Facing::Down);
    } else {
      m_player.placeOnTile({3, 3}, Facing::Down);
    }
    m_camera.centerOn(m_player.worldCenter());
  }

  m_spiders.reserve(10);

  const auto ts = m_map.tileSizePixels();
  for (int i = 1; i <= 8; ++i) {
    const std::string spName = "spider_" + std::to_string(i);
    if (const auto* sp = m_map.findSpawn(spName)) {
      auto spider = std::make_unique<Spider>();
      if (spider->loadTexture("assets/sprites/enemies/spider.png")) {
        spider->placeOnTile({static_cast<int>(sp->position.x / ts.x),
                             static_cast<int>(sp->position.y / ts.y)},
                            ts);
        spider->setScale(2.f);
        m_spiders.push_back(std::move(spider));
      }
    }
  }
  m_spidersTotal = static_cast<int>(m_spiders.size());

  if (const auto* sp = m_map.findSpawn("spider_boss")) {
    m_boss = std::make_unique<BossSpider>();
    m_boss->placeAt(sp->position);
    ++m_spidersTotal;
  }

  for (const auto& obj : m_map.interactables()) {
    if (obj.props.count("type") && obj.props.at("type") == "chest") {
      ChestData chest;
      chest.position = {obj.position.x + obj.size.x / 3.f,
                        obj.position.y + obj.size.y / 3.f};
      chest.opened = false;
      chest.loot = (obj.name == "chest_1" || obj.name == "chest_3" ||
                    obj.name == "chest_5")
                       ? ChestData::LootType::Heal
                       : ChestData::LootType::FearReduce;
      m_chests.push_back(chest);
    }
  }
}

void ArachnophobiaScene::handleEvent(const sf::Event& event) {
  if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
    if (DialogueManager::instance().isVisible()) {
      BattleAction chosen = BattleAction::None;
      if (key->code == sf::Keyboard::Key::Num1)
        chosen = BattleAction::Attack;
      else if (key->code == sf::Keyboard::Key::Num2)
        chosen = BattleAction::Dodge;
      else if (key->code == sf::Keyboard::Key::Num3)
        chosen = BattleAction::Talk;

      if (chosen != BattleAction::None) {
        m_pendingAction = chosen;
        DialogueManager::instance().hide();
        resolveBossAction();
        m_waitingForClose = true;
        return;
      }

      if (key->code == sf::Keyboard::Key::E ||
          key->code == sf::Keyboard::Key::Enter) {
        DialogueManager::instance().hide();
        if (m_waitingForClose) {
          m_waitingForClose = false;
          m_currentTurn = TurnState::MonstersTurn;
        }
      }
      return;
    }

    if (m_currentTurn == TurnState::PlayerTurn) {
      if (key->code == sf::Keyboard::Key::D ||
          key->code == sf::Keyboard::Key::Right)
        m_nextDirection = Facing::Right;
      else if (key->code == sf::Keyboard::Key::A ||
               key->code == sf::Keyboard::Key::Left)
        m_nextDirection = Facing::Left;
      else if (key->code == sf::Keyboard::Key::S ||
               key->code == sf::Keyboard::Key::Down)
        m_nextDirection = Facing::Down;
      else if (key->code == sf::Keyboard::Key::W ||
               key->code == sf::Keyboard::Key::Up)
        m_nextDirection = Facing::Up;
    }

    if (key->code == sf::Keyboard::Key::E &&
        m_currentTurn == TurnState::PlayerTurn) {
      tryEngageBoss();

      const auto playerTile = m_player.currentTile();
      const auto ts = m_map.tileSizePixels();
      for (auto& chest : m_chests) {
        if (chest.opened) continue;
        const int cx = static_cast<int>(chest.position.x / ts.x);
        const int cy = static_cast<int>(chest.position.y / ts.y);
        if (std::abs(playerTile.x - cx) <= 3 &&
            std::abs(playerTile.y - cy) <= 3) {
          chest.opened = true;
          if (chest.loot == ChestData::LootType::Heal) {
            m_player.heal(3);
            DialogueManager::instance().showSingleReplica(
                "Сундук", "Ты нашёл бинты.\n+3 HP");
          } else {
            m_fearTimer = std::max(0.f, m_fearTimer - 8.f);
            DialogueManager::instance().showSingleReplica(
                "Сундук", "Успокоительное...\nСтрах отступил.");
          }
          break;
        }
      }
    }

    if (key->code == sf::Keyboard::Key::Space &&
        m_currentTurn == TurnState::PlayerTurn) {
      const auto pt = m_player.currentTile();
      for (auto& sp : m_spiders) {
        if (sp->isDead()) continue;
        const auto st = sp->currentTile();
        if (std::abs(pt.x - st.x) <= 1 && std::abs(pt.y - st.y) <= 1) {
          sp->takeDamage(3);
          if (sp->isDead()) ++m_spidersKilled;
          m_currentTurn = TurnState::MonstersTurn;
          break;
        }
      }
    }
  }
}

void ArachnophobiaScene::tryEngageBoss() {
  if (!m_boss || m_boss->isDead()) return;

  const sf::Vector2f diff = m_boss->position() - m_player.worldCenter();
  const float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

  if (dist <= m_boss->interactRadius()) {
    m_bossEngaged = true;
    DialogueManager::instance().showSingleReplica(
        "Арахна",
        "Ты осмелился войти в мои сети?\n"
        "[1] Атаковать\n"
        "[2] Уклониться\n"
        "[3] Говорить");
  }
}

void ArachnophobiaScene::resolveBossAction() {
  if (!m_boss) return;
  switch (m_pendingAction) {
    case BattleAction::Attack:
      m_boss->takeDamage(2);
      if (m_boss->isDead()) {
        ++m_spidersKilled;
        DialogueManager::instance().showSingleReplica(
            "Арахна",
            "Н-нет... невозможно...\n"
            "Ты победил меня... страх отступает...\n"
            "[E] Закрыть");
      } else {
        m_player.takeDamage(2);
        DialogueManager::instance().showSingleReplica(
            "Арахна",
            "Ты осмелился ударить меня?!\n"
            "HP: " +
                std::to_string(m_boss->hp()) + "/" +
                std::to_string(m_boss->maxHp()) +
                "\n"
                "[E] Закрыть");
      }
      break;

    case BattleAction::Dodge:
      DialogueManager::instance().showSingleReplica(
          "Арахна",
          "Трус. Ты уклонился... на этот раз.\n"
          "Но паутина везде.\n"
          "[E] Закрыть");
      break;

    case BattleAction::Talk:
      m_fearTimer = std::max(0.f, m_fearTimer - 5.f);
      DialogueManager::instance().showSingleReplica("Арахна",
                                                    "Слова... Любопытно.\n"
                                                    "Страх чуть отступил.\n"
                                                    "[E] Закрыть");
      break;

    default:
      break;
  }
  m_pendingAction = BattleAction::None;
}

void ArachnophobiaScene::checkOutcome() {
  if (m_finished) return;

  if (!m_player.isAlive()) {
    m_finished = true;
    m_hasPendingOutcome = true;
    m_pendingOutcome = NightmareOutcome::Failure;
    return;
  }
  if (m_spidersTotal > 0 && m_spidersKilled >= m_spidersTotal) {
    m_finished = true;
    m_hasPendingOutcome = true;
    m_pendingOutcome = NightmareOutcome::Therapy;
    return;
  }
  if (m_fearTimer >= m_fearMax) {
    m_finished = true;
    m_hasPendingOutcome = true;
    m_pendingOutcome = NightmareOutcome::Failure;
    return;
  }
  if (m_escapeTimer >= m_escapeRequired) {
    m_finished = true;
    m_hasPendingOutcome = true;
    m_pendingOutcome = NightmareOutcome::Neutral;
  }
}

void ArachnophobiaScene::update(float deltaTime) {
  if (!m_mapReady || m_finished) return;
  if (DialogueManager::instance().isVisible()) return;

  if (m_currentTurn == TurnState::PlayerTurn) {
    if (!m_player.isMoving() && m_nextDirection.has_value()) {
      m_player.tryStartStep(m_nextDirection.value(), m_map);
      m_nextDirection.reset();
    }
  }

  if (m_player.isMoving()) m_nextDirection.reset();

  const bool wasMoving = m_player.isMoving();
  m_player.update(deltaTime);
  m_camera.centerOn(m_player.worldCenter());

  if (wasMoving && !m_player.isMoving() &&
      m_currentTurn == TurnState::PlayerTurn) {
    m_currentTurn = TurnState::MonstersTurn;
  }

  const auto pt = m_player.currentTile();
  bool spiderNearby = false;

  if (m_currentTurn == TurnState::MonstersTurn) {
    for (auto& sp : m_spiders) {
      if (sp->isDead()) continue;
      sp->stepTowardPlayer(pt, m_map);
      sp->update(deltaTime, {});
      const auto st = sp->currentTile();
      if (std::abs(st.x - pt.x) <= 1 && std::abs(st.y - pt.y) <= 1) {
        spiderNearby = true;
        m_player.takeDamage(1);
      }
    }
    if (m_boss && !m_boss->isDead()) {
      const sf::Vector2f diff = m_boss->position() - m_player.worldCenter();
      if (std::sqrt(diff.x * diff.x + diff.y * diff.y) <
          m_boss->interactRadius())
        spiderNearby = true;
    }
    m_currentTurn = TurnState::CheckOutcomes;
  } else {
    for (auto& sp : m_spiders) {
      if (!sp->isDead()) {
        sp->update(deltaTime, {});
        const auto st = sp->currentTile();
        if (std::abs(st.x - pt.x) <= 1 && std::abs(st.y - pt.y) <= 1)
          spiderNearby = true;
      }
    }
    if (m_boss && !m_boss->isDead()) {
      const sf::Vector2f diff = m_boss->position() - m_player.worldCenter();
      if (std::sqrt(diff.x * diff.x + diff.y * diff.y) <
          m_boss->interactRadius())
        spiderNearby = true;
    }
  }

  if (spiderNearby)
    m_fearTimer += deltaTime;
  else
    m_fearTimer = std::max(0.f, m_fearTimer - deltaTime * 0.5f);

  if (m_currentTurn == TurnState::CheckOutcomes) {
    checkOutcome();
    if (!m_finished) m_currentTurn = TurnState::PlayerTurn;
  } else {
    checkOutcome();
  }

  if (m_hasPendingOutcome && m_onExit) {
    m_hasPendingOutcome = false;
    m_onExit(m_pendingOutcome);
  }
}

void ArachnophobiaScene::renderHUD(sf::RenderWindow& window) {
  if (!m_fontLoaded) return;
  const float winW = static_cast<float>(window.getSize().x);
  const float winH = static_cast<float>(window.getSize().y);

  sf::Text spiderCount(m_font);
  spiderCount.setString("Spiders: " +
                        std::to_string(m_spidersTotal - m_spidersKilled));
  spiderCount.setCharacterSize(12u);
  spiderCount.setFillColor(sf::Color(220, 180, 255));
  spiderCount.setPosition({12.f, 12.f});
  window.draw(spiderCount);

  sf::Text hpText(m_font);
  hpText.setString("HP: " + std::to_string(m_player.getHP()) + " / " +
                   std::to_string(m_player.getMaxHP()));
  hpText.setCharacterSize(12u);
  hpText.setFillColor(sf::Color(255, 90, 90));
  hpText.setPosition({12.f, 32.f});
  window.draw(hpText);

  if (m_boss && !m_boss->isDead()) {
    sf::Text bossHp(m_font);
    bossHp.setString("ARACHNA: " + std::to_string(m_boss->hp()) + "/" +
                     std::to_string(m_boss->maxHp()));
    bossHp.setCharacterSize(10u);
    bossHp.setFillColor(sf::Color(255, 80, 120));
    bossHp.setPosition({winW / 2.f - 70.f, 12.f});
    window.draw(bossHp);
  }

  sf::Text hint(m_font);
  hint.setString("WASD-move  SPACE-attack  E-interact");
  hint.setCharacterSize(9u);
  hint.setFillColor(sf::Color(180, 160, 200, 180));
  hint.setPosition({12.f, winH - 20.f});
  window.draw(hint);

  const float barW = 180.f;
  const float barH = 10.f;
  const float fearPct = std::min(m_fearTimer / m_fearMax, 1.f);

  sf::RectangleShape barBg({barW, barH});
  barBg.setFillColor(sf::Color(30, 10, 30, 200));
  barBg.setOutlineColor(sf::Color(160, 80, 200));
  barBg.setOutlineThickness(1.f);
  barBg.setPosition({winW - barW - 12.f, 12.f});
  window.draw(barBg);

  sf::RectangleShape barFill({barW * fearPct, barH});
  barFill.setFillColor(sf::Color(200, 50, 200, 220));
  barFill.setPosition({winW - barW - 12.f, 12.f});
  window.draw(barFill);

  sf::Text fearLabel(m_font);
  fearLabel.setString("LUCIDITY");
  fearLabel.setCharacterSize(9u);
  fearLabel.setFillColor(sf::Color(200, 150, 255, 200));
  fearLabel.setPosition({winW - barW - 12.f, 26.f});
  window.draw(fearLabel);
}

void ArachnophobiaScene::render(sf::RenderWindow& window) {
  window.clear(sf::Color(8, 5, 18));
  if (!m_mapReady) return;

  window.setView(m_camera.view());
  m_map.drawBackground(window);
  for (const auto& sp : m_spiders) window.draw(*sp);
  if (m_boss) window.draw(*m_boss);
  window.draw(m_player);
  m_map.drawDecorAbove(window);

  window.setView(window.getDefaultView());
  renderHUD(window);

  const sf::View& cv = m_camera.view();
  const sf::Vector2f wc = m_player.worldCenter();
  const sf::Vector2f vc = cv.getCenter();
  const sf::Vector2f vs = cv.getSize();
  const float playerScreenY = (wc.y - vc.y + vs.y / 2.f) *
                              (static_cast<float>(window.getSize().y) / vs.y);

  DialogueManager::instance().draw(window, m_font, playerScreenY);
}

}  // namespace somnia
