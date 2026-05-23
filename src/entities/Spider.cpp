#include "entities/Spider.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "world/TileMap.h"

namespace somnia {

bool Spider::loadTexture(const std::string& pngPath) {
  m_sprite.reset();
  m_texture.reset();

  m_texture = std::make_shared<sf::Texture>();
  if (!m_texture->loadFromFile(pngPath)) {
    std::cerr << "[Spider] cannot load " << pngPath << '\n';
    m_texture.reset();
    return false;
  }
  m_texture->setSmooth(false);
  m_sprite = std::make_unique<sf::Sprite>(*m_texture);
  m_sprite->setOrigin({2.f, 2.f});
  return true;
}

void Spider::placeOnTile(sf::Vector2i tile, sf::Vector2u tileSize) {
  m_tile = tile;
  m_tileSize = tileSize;
  m_visualPos = {static_cast<float>(tile.x * tileSize.x) + tileSize.x / 2.f,
                 static_cast<float>(tile.y * tileSize.y) + tileSize.y / 2.f};
  if (m_sprite) m_sprite->setPosition(m_visualPos);
}

void Spider::setScale(float scale) {
  if (m_sprite) m_sprite->setScale({scale, scale});
}

void Spider::stepTowardPlayer(sf::Vector2i playerTile, const TileMap& map) {
  if (isDead() || m_isAnimating) return;

  const int dx = playerTile.x - m_tile.x;
  const int dy = playerTile.y - m_tile.y;

  if (std::abs(dx) <= 1 && std::abs(dy) <= 1) return;

  sf::Vector2i next = m_tile;
  if (std::abs(dx) >= std::abs(dy))
    next.x += (dx > 0) ? 1 : -1;
  else
    next.y += (dy > 0) ? 1 : -1;

  if (!map.isBlocked(next.x, next.y)) {
    m_tile = next;
    m_isAnimating = true;
    m_animTimer = 0.f;
  }
}

void Spider::update(float deltaTime, sf::Vector2f /*unused*/) {
  if (!m_sprite || isDead()) return;

  if (m_isAnimating) {
    m_animTimer += deltaTime;
    const float t = std::min(m_animTimer / kAnimDuration, 1.f);

    const sf::Vector2f target = {
        static_cast<float>(m_tile.x * m_tileSize.x) + m_tileSize.x / 2.f,
        static_cast<float>(m_tile.y * m_tileSize.y) + m_tileSize.y / 2.f};

    m_visualPos = m_visualPos + (target - m_visualPos) * t;
    m_sprite->setPosition(m_visualPos);

    if (t >= 1.f) {
      m_visualPos = target;
      m_isAnimating = false;
    }
  }
}

bool Spider::takeDamage(int dmg) {
  m_hp -= dmg;
  return isDead();
}

void Spider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (m_sprite && !isDead()) target.draw(*m_sprite, states);
}

}  // namespace somnia
