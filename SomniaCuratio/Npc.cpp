#include "Npc.h"

#include <ctime>
#include <random>

Npc::Npc(const std::string& name, const std::string& texturePath)
    : m_name(name), m_texturePath(texturePath), m_sprite(m_texture) {}

bool Npc::load() {
  if (!m_texture.loadFromFile(m_texturePath)) return false;
  m_sprite.setTexture(m_texture, true);
  return true;
}

void Npc::setPosition(sf::Vector2f pos) { m_sprite.setPosition(pos); }

void Npc::draw(sf::RenderWindow& window) { window.draw(m_sprite); }

void Npc::addReplica(const std::string& text) { m_replicas.push_back(text); }

std::string Npc::getRandomReplica() {
  if (m_replicas.empty()) return "...";
  static std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
  std::uniform_int_distribution<> dis(0,
                                      static_cast<int>(m_replicas.size() - 1));
  return m_replicas[dis(gen)];
}
