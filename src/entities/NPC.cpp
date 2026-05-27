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

  m_sprite->setTextureRect(
      sf::IntRect({m_textureRectLeft, m_textureRectTop},
                  {m_textureRectWidth, m_textureRectHeight}));

  m_sprite->setOrigin({m_spriteOriginX, m_spriteOriginY});
  return true;
}

void NPC::placeAt(sf::Vector2f worldPos) {
  if (m_sprite) m_sprite->setPosition(worldPos);
}

void NPC::setScale(float scale) {
  if (m_sprite) m_sprite->setScale({scale, scale});
}

// —сылки RenderTarget и RenderStates об€зательны дл€ переопределени€
// виртуального метода sf::Drawable::draw
void NPC::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (m_sprite) target.draw(*m_sprite, states);
}

}  // namespace somnia
