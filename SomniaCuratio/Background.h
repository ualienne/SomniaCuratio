#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Background {
 public:
  Background();
  bool load(const std::string& path, sf::Vector2u windowSize);
  void draw(sf::RenderWindow& window);

 private:
  std::unique_ptr<sf::Texture> m_texture;
  std::unique_ptr<sf::Sprite> m_sprite;
};

#endif