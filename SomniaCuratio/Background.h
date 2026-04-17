#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <string>

class Background {
 public:
  Background();
  bool load(const std::string& filename, sf::Vector2u windowSize);
  void draw(sf::RenderWindow& window);

 private:
  sf::Texture m_texture;//Ќужно дл€ фона, не трогать иначе баги пойдут
  sf::Sprite m_sprite;//Ќужно дл€ фона, не трогать иначе баги пойдут
};

#endif