#ifndef DIALOGUE_MANAGER_H
#define DIALOGUE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

class DialogueManager {
 public:
  static DialogueManager& instance() {
    static DialogueManager inst;
    return inst;
  }

  // Метод для показа одной случайной реплики
  void showSingleReplica(const std::string& speaker, const std::string& text);

  void hide() { m_isVisible = false; }
  bool isVisible() const { return m_isVisible; }

  void draw(sf::RenderWindow& window, const sf::Font& font);

 private:
  DialogueManager() : m_isVisible(false) {} 

  std::string m_speakerName;
  std::string m_currentLine;
  bool m_isVisible;
};

#endif