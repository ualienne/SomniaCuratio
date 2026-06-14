#include "entities/Player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>
#include <iostream>

#include "world/TileMap.h"

namespace somnia {

Player::Player() = default;

bool Player::loadTexture(const std::string& pngPath) {
  m_texture = std::make_shared<sf::Texture>();
  if (!m_texture->loadFromFile(pngPath)) {
    std::cerr << "[Player] cannot load " << pngPath << '\n';
    m_texture.reset();
    return false;
  }
  m_texture->setSmooth(false);
  m_sprite = std::make_unique<sf::Sprite>(*m_texture);
  refreshSprite();
  return true;
}

void Player::placeOnTile(sf::Vector2i tile, Facing facing) {
  m_currentTile = tile;
  m_previousTile = tile;
  m_facing = facing;
  m_stepProgress = m_stepDuration;
  refreshSprite();
}

sf::Vector2f Player::worldCenter() const {
  const float t = (m_stepDuration > m_minStepDurationLimit)
                      ? std::min(m_stepProgress / m_stepDuration,
                                 m_interpolationProgressMax)
                      : m_interpolationProgressMax;

  const float px =
      (static_cast<float>(m_previousTile.x) * (m_interpolationProgressMax - t) +
       static_cast<float>(m_currentTile.x) * t) *
          m_tileSize.x +
      m_tileSize.x / m_tileHalfDivider;
  const float py =
      (static_cast<float>(m_previousTile.y) * (m_interpolationProgressMax - t) +
       static_cast<float>(m_currentTile.y) * t) *
          m_tileSize.y +
      m_tileSize.y / m_tileHalfDivider;
  return {px, py};
}

void Player::tryStartStep(Facing dir, const TileMap& map) {
  if (isMoving()) return;

  m_facing = dir;

  sf::Vector2i target = m_currentTile;
  switch (dir) {
    case Facing::Right:
      target.x += m_tileStepOffset;
      break;
    case Facing::Left:
      target.x -= m_tileStepOffset;
      break;
    case Facing::Up:
      target.y -= m_tileStepOffset;
      break;
    case Facing::Down:
      target.y += m_tileStepOffset;
      break;
  }

  if (map.isBlocked(target.x, target.y)) {
    refreshSprite();
    return;
  }

  m_previousTile = m_currentTile;
  m_currentTile = target;
  m_stepProgress = 0.f;
  refreshSprite();
}

void Player::update(float deltaTime) {
  if (isMoving()) {
    m_stepProgress += deltaTime;
    if (m_stepProgress > m_stepDuration) {
      m_stepProgress = m_stepDuration;
      m_previousTile = m_currentTile;
    }
  }
  refreshSprite();
}

void Player::refreshSprite() {
  if (!m_sprite) return;

  const int frameIdx = static_cast<int>(m_facing);
  m_sprite->setTextureRect(sf::IntRect(
      {frameIdx * kFrameWidth, m_textureRectTop}, {kFrameWidth, kFrameHeight}));

  m_sprite->setOrigin({kFrameWidth / m_tileHalfDivider,
                       kFrameHeight - kFrameWidth / m_tileHalfDivider});

  m_sprite->setScale({m_spriteScaleX, m_spriteScaleY});

  const auto world = worldCenter();
  m_sprite->setPosition(world);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (m_sprite) target.draw(*m_sprite, states);
}

}  // namespace somnia