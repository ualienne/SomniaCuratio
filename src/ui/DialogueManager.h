#ifndef SOMNIA_UI_DIALOGUE_MANAGER_H
#define SOMNIA_UI_DIALOGUE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

namespace somnia {

class DialogueManager {
 public:
  static DialogueManager& instance() {
    static DialogueManager inst;
    return inst;
  }

  void showSingleReplica(const std::string& speaker, const std::string& text);
  void hide() { m_isVisible = false; }
  bool isVisible() const { return m_isVisible; }

  void draw(sf::RenderWindow& window, const sf::Font& font,
            float playerScreenY) const;

 private:
  DialogueManager() = default;
  DialogueManager(const DialogueManager&) = delete;
  DialogueManager& operator=(const DialogueManager&) = delete;

  std::string m_speakerName;
  std::string m_currentLine;
  bool m_isVisible = false;
};

}  // namespace somnia

#endif  // SOMNIA_UI_DIALOGUE_MANAGER_H
