#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/NightmareOutcome.h"
#include "entities/BossSpider.h"
#include "entities/Player.h"
#include "entities/Spider.h"
#include "scenes/Scene.h"
#include "ui/DialogueManager.h"
#include "world/Camera.h"
#include "world/TileMap.h"

namespace somnia {

class ArachnophobiaScene : public Scene {
 public:
  using OutcomeCallback = std::function<void(NightmareOutcome)>;

  ArachnophobiaScene(sf::RenderWindow& window,
                     const std::string& spawnPointName, OutcomeCallback onExit);

  void handleEvent(const sf::Event& event) override;
  void update(float deltaTime) override;
  void render(sf::RenderWindow& window) override;

 private:
  enum class TurnState { PlayerTurn, MonstersTurn, CheckOutcomes };
  enum class BattleAction { None, Attack, Dodge, Talk };

  void checkOutcome();
  void renderHUD(sf::RenderWindow& window);
  void tryEngageBoss();
  void resolveBossAction();

  sf::RenderWindow& m_window;
  sf::Font m_font;
  bool m_fontLoaded = false;

  bool m_waitingForClose = false;

  TileMap m_map;
  Camera m_camera;
  Player m_player;

  std::string m_spawnPointName;
  OutcomeCallback m_onExit;

  std::vector<std::unique_ptr<Spider>> m_spiders;

  std::unique_ptr<BossSpider> m_boss;
  bool m_bossEngaged = false;
  BattleAction m_pendingAction = BattleAction::None;

  std::optional<Facing> m_nextDirection;
  TurnState m_currentTurn = TurnState::PlayerTurn;

  int m_spidersTotal = 0;
  int m_spidersKilled = 0;
  float m_fearTimer = 0.f;
  float m_fearMax = 30.f;
  float m_escapeTimer = 0.f;
  float m_escapeRequired = 5.f;
  bool m_finished = false;
  bool m_mapReady = false;

  bool m_hasPendingOutcome = false;
  NightmareOutcome m_pendingOutcome{};

  struct ChestData {
    sf::Vector2f position;
    bool opened = false;
    enum class LootType { Heal, FearReduce } loot;
  };
  std::vector<ChestData> m_chests;
};

}  // namespace somnia
