#include "entities/NPC.h"

#include <iostream>

namespace somnia {

bool NPC::loadTexture(const std::string& pngPath) {
  m_texture = std::make_shared<sf::Texture>();
  if (!m_texture->loadFromFile(pngPath)) {
    std::cerr << "[NPC] cannot load " << pngPath << '\n';
    m_texture.reset();
    return false;
  }
  m_texture->setSmooth(false);

  m_sprite = std::make_unique<sf::Sprite>(*m_texture);

  m_sprite->setTextureRect(sf::IntRect({48, 0}, {16, 32}));
  m_sprite->setOrigin({8.f, 24.f});
  return true;
}

void NPC::placeAt(sf::Vector2f worldPos) {
  if (m_sprite) m_sprite->setPosition(worldPos);
}

void NPC::setScale(float scale) {
  if (m_sprite) m_sprite->setScale({scale, scale});
}

void NPC::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (m_sprite) target.draw(*m_sprite, states);
}

}  // namespace somnia
