#include "world/Camera.h"

#include <algorithm>

namespace somnia {

Camera::Camera(sf::Vector2f viewportSize) {
  constexpr float kZoom = 0.75f;

  const sf::Vector2f viewSize = {viewportSize.x * kZoom,
                                 viewportSize.y * kZoom};

  m_view = sf::View(
      sf::FloatRect({m_rectLeftInit, m_rectTopInit}, {viewSize.x, viewSize.y}));
  m_view.setCenter({viewSize.x / m_halfDivider, viewSize.y / m_halfDivider});

  m_viewportSize = viewSize;
}

void Camera::centerOn(sf::Vector2f worldPoint) {
  const float halfW = m_viewportSize.x / m_halfDivider;
  const float halfH = m_viewportSize.y / m_halfDivider;

  float cx = worldPoint.x;
  float cy = worldPoint.y;

  if (m_worldSize.x <= m_viewportSize.x) {
    cx = m_worldSize.x / m_halfDivider;
  } else {
    cx = std::clamp(cx, halfW, m_worldSize.x - halfW);
  }

  if (m_worldSize.y <= m_viewportSize.y) {
    cy = m_worldSize.y / m_halfDivider;
  } else {
    cy = std::clamp(cy, halfH, m_worldSize.y - halfH);
  }

  m_view.setCenter({cx, cy});
}

}  // namespace somnia