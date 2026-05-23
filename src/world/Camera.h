#ifndef SOMNIA_WORLD_CAMERA_H
#define SOMNIA_WORLD_CAMERA_H

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
  sf::View m_view;
  sf::Vector2f m_viewportSize{0.f, 0.f};
  sf::Vector2f m_worldSize{0.f, 0.f};
};

}  // namespace somnia

#endif  // SOMNIA_WORLD_CAMERA_H
