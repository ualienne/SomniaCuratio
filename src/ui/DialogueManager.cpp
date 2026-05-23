#include "ui/DialogueManager.h"

namespace somnia {

void DialogueManager::showSingleReplica(const std::string& speaker,
                                        const std::string& text) {
  m_speakerName = speaker;
  m_currentLine = text;
  m_isVisible = true;
}

void DialogueManager::draw(sf::RenderWindow& window, const sf::Font& font,
                           float playerScreenY) const {
  if (!m_isVisible) return;

  const float winW = static_cast<float>(window.getSize().x);
  const float winH = static_cast<float>(window.getSize().y);

  constexpr float kBoxW = 620.f;
  constexpr float kBoxH = 130.f;
  constexpr float kMargin = 35.f;
  constexpr float kPad = 16.f;

  const float boxX = (winW - kBoxW) / 2.f;

  const bool showTop = (playerScreenY > winH * 0.5f);
  const float boxY = showTop ? kMargin : (winH - kBoxH - kMargin);

  sf::RectangleShape box({kBoxW, kBoxH});
  box.setFillColor(sf::Color(10, 12, 28, 225));
  box.setOutlineColor(sf::Color(160, 140, 220, 255));
  box.setOutlineThickness(2.f);
  box.setPosition({boxX, boxY});
  window.draw(box);

  sf::Text nameText(font);
  nameText.setString(
      sf::String::fromUtf8(m_speakerName.begin(), m_speakerName.end()));
  nameText.setCharacterSize(16u);
  nameText.setFillColor(sf::Color(200, 180, 255, 255));
  nameText.setStyle(sf::Text::Bold);
  nameText.setPosition({boxX + kPad, boxY + kPad});
  window.draw(nameText);

  sf::Text lineText(font);
  lineText.setString(
      sf::String::fromUtf8(m_currentLine.begin(), m_currentLine.end()));
  lineText.setCharacterSize(14u);
  lineText.setFillColor(sf::Color(220, 220, 240, 255));
  lineText.setPosition({boxX + kPad, boxY + kPad + 26.f});
  window.draw(lineText);

  sf::Text hintText(font);
  hintText.setString("[ E ] close");
  hintText.setCharacterSize(11u);
  hintText.setFillColor(sf::Color(130, 120, 180, 200));

  const sf::FloatRect hintBounds = hintText.getLocalBounds();
  hintText.setPosition({boxX + kBoxW - hintBounds.size.x - kPad,
                        boxY + kBoxH - hintBounds.size.y - kPad});
  window.draw(hintText);
}

}  // namespace somnia
