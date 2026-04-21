#include "Background.h"

Background::Background() : m_texture(nullptr), m_sprite(nullptr) {}

bool Background::load(const std::string& path, sf::Vector2u windowSize) {
  m_texture = std::make_unique<sf::Texture>();

  if (!m_texture->loadFromFile(path)) {
    return false;
  }

  m_sprite = std::make_unique<sf::Sprite>(*m_texture);

  // Масштабирование
  sf::Vector2u textureSize = m_texture->getSize();
  float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
  float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
  m_sprite->setScale({scaleX, scaleY});

  return true;
}

void Background::draw(sf::RenderWindow& window) {
  if (m_sprite) {
    window.draw(*m_sprite);
  }
}
