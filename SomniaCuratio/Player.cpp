#include "Player.h"

Player::Player() : m_sprite(m_texture) {}

bool Player::load(const std::string& filename) {
  if (!m_texture.loadFromFile(filename)) {
    return false;
  }
  m_sprite.setTexture(m_texture, true);
  m_sprite.setPosition(
      {220.0f, 550.0f});  // Ульяна прошу не трогай эти числа, это спавн прямо
                          // на входе. Мне пришлось вручную подбирать значения
  return true;
}

void Player::handleInput(sf::Keyboard::Key key) {
  if (key == sf::Keyboard::Key::W) m_sprite.move({0.f, -m_step});
  if (key == sf::Keyboard::Key::S) m_sprite.move({0.f, m_step});
  if (key == sf::Keyboard::Key::A) m_sprite.move({-m_step, 0.f});
  if (key == sf::Keyboard::Key::D) m_sprite.move({m_step, 0.f});
}  // Наш любимый WASD. Думаю ограничения по движению можно сдесь же и сделать

void Player::draw(sf::RenderWindow& window) { window.draw(m_sprite); }
