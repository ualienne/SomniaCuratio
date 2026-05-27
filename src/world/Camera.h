#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

namespace somnia {

class Camera {
 public:
  explicit Camera(sf::Vector2f viewportSize);

  void setWorldSize(sf::Vector2f worldSize) { m_worldSize = worldSize; }

  void centerOn(sf::Vector2f worldPoint);

  const sf::View& view() const { return m_view; }

 private:
  static constexpr float m_rectLeftInit = 0.f;
  static constexpr float m_rectTopInit = 0.f;
  static constexpr float m_halfDivider = 2.f;

  sf::View m_view;
  sf::Vector2f m_viewportSize{0.f, 0.f};
  sf::Vector2f m_worldSize{0.f, 0.f};
};

}  // namespace somnia
