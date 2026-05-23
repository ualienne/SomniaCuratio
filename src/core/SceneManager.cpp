#include "core/SceneManager.h"

#include "scenes/Scene.h"

namespace somnia {

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() = default;

void SceneManager::pushScene(std::unique_ptr<Scene> scene) {
  m_pending.push_back({OpKind::Push, std::move(scene)});
}

void SceneManager::popScene() { m_pending.push_back({OpKind::Pop, nullptr}); }

void SceneManager::replaceScene(std::unique_ptr<Scene> scene) {
  m_pending.push_back({OpKind::Replace, std::move(scene)});
}

void SceneManager::resetTo(std::unique_ptr<Scene> scene) {
  m_pending.push_back({OpKind::Reset, std::move(scene)});
}

bool SceneManager::empty() const {
  return m_scenes.empty() && m_pending.empty();
}

void SceneManager::handleEvent(const sf::Event& event) {
  if (m_scenes.empty()) return;
  m_scenes.back()->handleEvent(event);
}

void SceneManager::update(float deltaTime) {
  flushPendingOps();

  if (m_scenes.empty()) return;
  m_scenes.back()->update(deltaTime);
}

void SceneManager::render(sf::RenderWindow& window) {
  for (const auto& scene : m_scenes) {
    scene->render(window);
  }
}

void SceneManager::flushPendingOps() {
  auto ops = std::move(m_pending);
  m_pending.clear();

  for (auto& op : ops) {
    switch (op.kind) {
      case OpKind::Push:
        if (!m_scenes.empty()) m_scenes.back()->onPause();
        m_scenes.push_back(std::move(op.scene));
        break;

      case OpKind::Pop:
        if (!m_scenes.empty()) m_scenes.pop_back();
        if (!m_scenes.empty()) m_scenes.back()->onResume();
        break;

      case OpKind::Replace:
        if (!m_scenes.empty()) m_scenes.pop_back();
        m_scenes.push_back(std::move(op.scene));
        break;

      case OpKind::Reset:
        m_scenes.clear();
        m_scenes.push_back(std::move(op.scene));
        break;
    }
  }
}

}  // namespace somnia
