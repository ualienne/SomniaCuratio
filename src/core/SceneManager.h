#ifndef SOMNIA_CORE_SCENE_MANAGER_H
#define SOMNIA_CORE_SCENE_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <vector>

namespace somnia {

class Scene;

class SceneManager {
 public:
  SceneManager();
  ~SceneManager();

  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  void pushScene(std::unique_ptr<Scene> scene);

  void popScene();

  void replaceScene(std::unique_ptr<Scene> scene);

  void resetTo(std::unique_ptr<Scene> scene);

  void handleEvent(const sf::Event& event);
  void update(float deltaTime);
  void render(sf::RenderWindow& window);

  bool empty() const;

 private:
  void flushPendingOps();

  std::vector<std::unique_ptr<Scene>> m_scenes;

  enum class OpKind { Push, Pop, Replace, Reset };
  struct PendingOp {
    OpKind kind;
    std::unique_ptr<Scene> scene;
  };
  std::vector<PendingOp> m_pending;
};

}  // namespace somnia

#endif  // SOMNIA_CORE_SCENE_MANAGER_H
