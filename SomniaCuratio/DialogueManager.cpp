#include "DialogueManager.h"

void DialogueManager::showSingleReplica(const std::string& speaker,
                                        const std::string& text) {
  m_speakerName = speaker;
  m_currentLine = text;
  m_isVisible = true;
}

void DialogueManager::draw(sf::RenderWindow& window, const sf::Font& font) {
  if (!m_isVisible) return;

  // Фон окна диалога
  sf::RectangleShape box({600.f, 120.f});
  box.setFillColor(sf::Color(0, 0, 0, 200));  // Полупрозрачный черный
  box.setOutlineColor(sf::Color::White);
  box.setOutlineThickness(2.f);
  box.setPosition({100.f, 450.f});
  window.draw(box);

  // Имя
  sf::Text nameText(font);
  nameText.setString(
      sf::String::fromUtf8(m_speakerName.begin(), m_speakerName.end()));
  nameText.setCharacterSize(20);
  nameText.setFillColor(sf::Color::Yellow);
  nameText.setPosition({120.f, 460.f});
  window.draw(nameText);

  // Текст реплики
  sf::Text lineText(font);
  lineText.setString(
      sf::String::fromUtf8(m_currentLine.begin(), m_currentLine.end()));
  lineText.setCharacterSize(18);
  lineText.setFillColor(sf::Color::White);
  lineText.setPosition({120.f, 490.f});
  window.draw(lineText);
}