#include "entities/Player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>  // std::min
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
  m_stepProgress = m_stepDuration;  // «стоим»
  refreshSprite();
}

sf::Vector2f Player::worldCenter() const {
  // Центральная точка спрайта — середина клетки, в которой он сейчас логически.
  // Во время интерполяции двигаем пропорционально.
  const float t = (m_stepDuration > 0.f)
                      ? std::min(m_stepProgress / m_stepDuration, 1.f)
                      : 1.f;

  const float px = (static_cast<float>(m_previousTile.x) * (1.f - t) +
                    static_cast<float>(m_currentTile.x) * t) *
                       m_tileSize.x +
                   m_tileSize.x / 2.f;
  const float py = (static_cast<float>(m_previousTile.y) * (1.f - t) +
                    static_cast<float>(m_currentTile.y) * t) *
                       m_tileSize.y +
                   m_tileSize.y / 2.f;
  return {px, py};
}

void Player::tryStartStep(Facing dir, const TileMap& map) {
  // Если ещё идём — не начинаем новый шаг. Это снимает нужду обрабатывать
  // «отменить шаг посередине». Tile-based движение = никогда не застрянем.
  if (isMoving()) return;

  m_facing = dir;  // можно «поворачиваться», даже если упираемся в стену

  // Куда хотим шагнуть.
  sf::Vector2i target = m_currentTile;
  switch (dir) {
    case Facing::Right:
      target.x += 3;
      break;
    case Facing::Left:
      target.x -= 3;
      break;
    case Facing::Up:
      target.y -= 3;
      break;
    case Facing::Down:
      target.y += 3;
      break;
  }

  // Проверка проходимости. Если упираемся — не двигаемся, но кадр
  // взгляда уже обновится (повернётся лицом).
  if (map.isBlocked(target.x, target.y)) {
    refreshSprite();
    return;
  }

  // Стартуем шаг: previous = где стояли, current = куда идём.
  m_previousTile = m_currentTile;
  m_currentTile = target;
  m_stepProgress = 0.f;
  refreshSprite();
}

void Player::update(float deltaTime) {
  if (isMoving()) {
    m_stepProgress += deltaTime;
    if (m_stepProgress > m_stepDuration) {
      m_stepProgress = m_stepDuration;  // фиксируем на финальном тайле
      m_previousTile = m_currentTile;   // шаг завершён
    }
  }
  refreshSprite();
}

void Player::refreshSprite() {
  if (!m_sprite) return;

  // Выбираем нужный кадр по Facing. Порядок: Right=0, Down=1, Up=2, Left=3.
  // Все кадры на одном листе по горизонтали, высота одна — m_kFrameHeight.
  const int frameIdx = static_cast<int>(m_facing);
  m_sprite->setTextureRect(
      sf::IntRect({frameIdx * kFrameWidth, 0}, {kFrameWidth, kFrameHeight}));

  // Спрайт 16×32. Ставим «ноги» в середину тайла, голова — выше.
  // Локальный origin кладём в (8, 24) — это центр нижней половины спрайта.
  // При 16x16 тайле: середина клетки по X, нижняя граница клетки по Y.
  m_sprite->setOrigin({kFrameWidth / 2.f, kFrameHeight - kFrameWidth / 2.f});

  m_sprite->setScale({4.f, 4.f});

  const auto world = worldCenter();
  m_sprite->setPosition(world);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (m_sprite) target.draw(*m_sprite, states);
}

}  // namespace somnia
