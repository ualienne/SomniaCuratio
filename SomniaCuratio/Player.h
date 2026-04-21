#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>

class Player {
 public:
  Player();
  bool load(const std::string& filename);
  void handleInput(sf::Keyboard::Key key);
  void draw(sf::RenderWindow& window);
  sf::Vector2f getPosition() const {
    return m_sprite.getPosition();
  }

 private:
  sf::Texture m_texture;//Нужно для героя, не трогать иначе баги пойдут
  sf::Sprite m_sprite;//Нужно для героя, не трогать иначе баги пойдут
  const float m_step = 32.0f;//Размер шага
};

#endif
