#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace somnia {

class Scene {
 public:
  virtual ~Scene() = default;

  virtual void handleEvent(const sf::Event& event) = 0;

  virtual void update(float deltaTime) = 0;

  virtual void render(sf::RenderWindow& window) = 0;

  virtual void onPause() {}

  virtual void onResume() {}

 protected:
  Scene() = default;

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;
};

}  // namespace somnia
