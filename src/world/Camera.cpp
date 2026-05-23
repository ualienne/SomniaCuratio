#include "world/Camera.h"

#include <algorithm>

namespace somnia {

Camera::Camera(sf::Vector2f viewportSize) {
  constexpr float kZoom = 0.75f;

  const sf::Vector2f viewSize = {viewportSize.x * kZoom,
                                 viewportSize.y * kZoom};

  m_view = sf::View(sf::FloatRect({0.f, 0.f}, {viewSize.x, viewSize.y}));
  m_view.setCenter({viewSize.x / 2.f, viewSize.y / 2.f});

  m_viewportSize = viewSize;
}

void Camera::centerOn(sf::Vector2f worldPoint) {
  const float halfW = m_viewportSize.x / 2.f;
  const float halfH = m_viewportSize.y / 2.f;

  float cx = worldPoint.x;
  float cy = worldPoint.y;

  if (m_worldSize.x <= m_viewportSize.x) {
    cx = m_worldSize.x / 2.f;
  } else {
    cx = std::clamp(cx, halfW, m_worldSize.x - halfW);
  }

  if (m_worldSize.y <= m_viewportSize.y) {
    cy = m_worldSize.y / 2.f;
  } else {
    cy = std::clamp(cy, halfH, m_worldSize.y - halfH);
  }

  m_view.setCenter({cx, cy});
}

}  // namespace somnia
