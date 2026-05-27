#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>

namespace somnia {

class TileMap;

enum class Facing : unsigned char { Right = 0, Up = 1, Left = 2, Down = 3 };

class Player : public sf::Drawable {
 public:
  Player();

  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;
  Player(Player&&) = default;
  Player& operator=(Player&&) = default;

  bool loadTexture(const std::string& pngPath);
  void placeOnTile(sf::Vector2i tile, Facing facing = Facing::Down);
  void setTileSize(sf::Vector2u tileSize) { m_tileSize = tileSize; }
  void tryStartStep(Facing dir, const TileMap& map);
  void update(float deltaTime);

  sf::Vector2i currentTile() const { return m_currentTile; }
  Facing facing() const { return m_facing; }
  sf::Vector2f worldCenter() const;
  bool isMoving() const { return m_stepProgress < m_stepDuration; }

  int getHP() const { return m_hp; }
  int getMaxHP() const { return m_maxHp; }
  bool isAlive() const { return m_hp > m_deadHpThreshold; }
  void takeDamage(int amount) {
    m_hp -= amount;
    if (m_hp < m_minHp) m_hp = m_minHp;
  }

  void heal(int amount) { m_hp = std::min(m_hp + amount, m_maxHp); }

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void refreshSprite();

  static constexpr int m_deadHpThreshold = 0;
  static constexpr int m_minHp = 0;

  static constexpr float m_minStepDurationLimit = 0.f;
  static constexpr float m_interpolationProgressMax = 1.f;
  static constexpr float m_tileHalfDivider = 2.f;

  static constexpr int m_tileStepOffset = 3;

  static constexpr int m_textureRectTop = 0;
  static constexpr float m_spriteScaleX = 4.f;
  static constexpr float m_spriteScaleY = 4.f;

  std::shared_ptr<sf::Texture> m_texture;
  std::unique_ptr<sf::Sprite> m_sprite;

  Facing m_facing = Facing::Down;
  sf::Vector2i m_currentTile{0, 0};
  sf::Vector2i m_previousTile{0, 0};
  float m_stepProgress = 1.f;
  float m_stepDuration = 0.18f;
  sf::Vector2u m_tileSize{16, 16};

  int m_hp = 10;
  int m_maxHp = 10;

  static constexpr int kFrameWidth = 16;
  static constexpr int kFrameHeight = 32;
};

}  // namespace somnia
