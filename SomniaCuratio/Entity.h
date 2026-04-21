#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
 public:
  virtual ~Entity() = default;
  virtual void update(float deltaTime) = 0;
  virtual void draw(sf::RenderWindow& window) = 0;

  void setPosition(sf::Vector2f pos) { m_position = pos; }
  sf::Vector2f getPosition() const { return m_position; }

 protected:
  sf::Vector2f m_position;
};

#endif