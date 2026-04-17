#include "Background.h"

Background::Background() : m_sprite(m_texture) {}

bool Background::load(const std::string& filename, sf::Vector2u windowSize) {
  if (!m_texture.loadFromFile(filename)) {
    return false;
  }
  m_sprite.setTexture(m_texture, true);

  float scaleX = static_cast<float>(windowSize.x) / m_texture.getSize().x;
  float scaleY = static_cast<float>(windowSize.y) / m_texture.getSize().y;
  m_sprite.setScale(
      {scaleX, scaleY});  // Подгон по размеру экрана. Думаю можно настроить
                          // так, чтобы сразу на весь экран открывалась игра

  return true;
}

void Background::draw(sf::RenderWindow& window) { window.draw(m_sprite); }
