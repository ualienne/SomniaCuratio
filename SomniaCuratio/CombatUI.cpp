#include "CombatUI.h"

#include "StatsManager.h"


CombatUI::CombatUI(sf::Font& font)
    : m_infoText(font), m_buttons{font, font, font, font} {
  m_border.setSize({700.f, 160.f});
  m_border.setFillColor(sf::Color::White);
  m_border.setPosition({50.f, 280.f});

  m_bg.setSize({694.f, 154.f});
  m_bg.setFillColor(sf::Color::Black);
  m_bg.setPosition({53.f, 283.f});

  // Полоска HP
  m_hpBarRed.setSize({100.f, 15.f});
  m_hpBarRed.setFillColor(sf::Color::Red);
  m_hpBarRed.setPosition({350.f, 360.f});

  m_hpBarYellow.setFillColor(sf::Color::Yellow);
  m_hpBarYellow.setPosition({350.f, 360.f});

  // Текст
  m_infoText.setCharacterSize(
      18);
  m_infoText.setFillColor(sf::Color::White);
  m_infoText.setPosition({75.f, 300.f});

  // Кнопки
  std::wstring names[4] = {L"1. АТАКА", L"2. ЗАЩИТА", L"3. ПРЕДМЕТ",
                           L"4. ПОЩАДА"};
  for (int i = 0; i < 4; i++) {
    m_buttons[i].setFont(font);
    m_buttons[i].setString(names[i]);
    m_buttons[i].setCharacterSize(18);
    m_buttons[i].setFillColor(sf::Color::White);
    m_buttons[i].setOutlineColor(sf::Color(255, 140, 0));
    m_buttons[i].setOutlineThickness(1.f);
    m_buttons[i].setPosition({70.f + (i * 170.f), 390.f});
  }
}

void CombatUI::draw(sf::RenderWindow& window, const CombatManager& combat) {
  window.draw(m_border);
  window.draw(m_bg);

  PlayerStats ps = StatsManager::load();
  float hpRatio = (ps.maxHp > 0) ? static_cast<float>(ps.hp) / ps.maxHp : 0.f;
  m_hpBarYellow.setSize({100.f * std::max(0.f, hpRatio), 15.f});
  window.draw(m_hpBarRed);
  window.draw(m_hpBarYellow);

  if (combat.getEnemy()) {
    std::wstring status;
    std::wstring pName(ps.name.begin(), ps.name.end());

    switch (combat.getState()) {
      case CombatState::PLAYER_TURN:
        if (!combat.getLastActionMessage().empty())
          status = combat.getLastActionMessage() + L"\nТвой ход, " + pName;
        else
          status = L"Твой ход, " + pName;
        break;
      case CombatState::ENEMY_TURN:
        status = L"Враг замахивается...";
        break;
      case CombatState::WIN:
        status = L"Победа!";
        break;
      case CombatState::LOSE:
        status = L"Ты проиграла...";
        break;
    }
    m_infoText.setString(status);
    window.draw(m_infoText);
  }

  for (int i = 0; i < 4; i++) window.draw(m_buttons[i]);
}
