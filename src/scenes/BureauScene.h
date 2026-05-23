#ifndef SOMNIA_SCENES_BUREAU_SCENE_H
#define SOMNIA_SCENES_BUREAU_SCENE_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/SceneManager.h"
#include "entities/NPC.h"
#include "entities/Player.h"
#include "scenes/Scene.h"
#include "world/Camera.h"
#include "world/TileMap.h"

namespace somnia {

class BureauScene : public Scene {
 public:
  BureauScene(sf::RenderWindow& window, const std::string& spawnPointName,
              SceneManager& sceneManager);

  void handleEvent(const sf::Event& event) override;
  void update(float deltaTime) override;
  void render(sf::RenderWindow& window) override;
  void onResume() override;

 private:
  std::optional<Facing> m_nextDirection;
  sf::RenderWindow& m_window;
  SceneManager& m_sceneManager;

  sf::Font m_font;
  bool m_fontLoaded = false;
  std::unique_ptr<sf::Text> m_helpText;

  TileMap m_map;
  Camera m_camera;
  Player m_player;

  std::string m_spawnPointName;
  std::vector<std::unique_ptr<NPC>> m_npcs;

  bool m_mapReady = false;
  std::string m_pendingMessage;
};

}  // namespace somnia

#endif  // SOMNIA_SCENES_BUREAU_SCENE_H
