#include "ui/Inventory.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "entities/Player.h"
#include "ui/DialogueManager.h"

namespace somnia {

    void Inventory::addItem(ItemType item) { m_items.push_back(item); }

    void Inventory::handleEvent(const sf::Event& event, Player& player,
        float& fearTimer, float fearTimerMinLimit,
        float chestFearReduceAmount,
        const std::string& invTitle,
        const std::string& msgHeal,
        const std::string& msgFear) {
        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::F) {
                m_isOpen = !m_isOpen;
                if (m_isOpen) {
                    m_selectedIndex = 0;
                }
                return;
            }

            if (!m_isOpen) return;

            if (!m_items.empty()) {
                if (key->code == sf::Keyboard::Key::W ||
                    key->code == sf::Keyboard::Key::Up) {
                    m_selectedIndex =
                        (m_selectedIndex - 1 + static_cast<int>(m_items.size())) %
                        static_cast<int>(m_items.size());
                }
                else if (key->code == sf::Keyboard::Key::S ||
                    key->code == sf::Keyboard::Key::Down) {
                    m_selectedIndex =
                        (m_selectedIndex + 1) % static_cast<int>(m_items.size());
                }
                else if (key->code == sf::Keyboard::Key::Enter ||
                    key->code == sf::Keyboard::Key::E) {
                    ItemType usedItem = m_items[m_selectedIndex];

                    m_items.erase(m_items.begin() + m_selectedIndex);

                    if (usedItem == ItemType::Heal) {
                        player.heal(m_healAmount);
                        DialogueManager::instance().showSingleReplica(invTitle, msgHeal);
                    }
                    else {
                        fearTimer =
                            std::max(fearTimerMinLimit, fearTimer - chestFearReduceAmount);
                        DialogueManager::instance().showSingleReplica(invTitle, msgFear);
                    }

                    if (m_selectedIndex >= static_cast<int>(m_items.size()) &&
                        !m_items.empty()) {
                        m_selectedIndex = static_cast<int>(m_items.size()) - 1;
                    }

                    m_isOpen = false;
                }
            }
        }
    }

    void Inventory::render(sf::RenderWindow& window, const sf::Font& font) const {
        if (!m_isOpen) return;

        const float winW = static_cast<float>(window.getSize().x);
        const float winH = static_cast<float>(window.getSize().y);

        const float boxX = (winW - m_boxWidth) / 2.f;
        const float boxY = (winH - m_boxHeight) / 2.f;

        sf::RectangleShape bg({ m_boxWidth, m_boxHeight });
        bg.setFillColor(sf::Color(15, 10, 25, 230));
        bg.setOutlineColor(sf::Color(160, 80, 200));
        bg.setOutlineThickness(2.f);
        bg.setPosition({ boxX, boxY });
        window.draw(bg);

        sf::Text title(font);
        title.setString("--- INVENTORY ---");
        title.setCharacterSize(m_charSize + 2);
        title.setFillColor(sf::Color(220, 180, 255));
        float titleX = boxX + (m_boxWidth - title.getLocalBounds().size.x) / 2.f;
        title.setPosition({ titleX, boxY + m_paddingY });
        window.draw(title);

        if (m_items.empty()) {
            sf::Text emptyText(font);
            emptyText.setString("[ Empty ]");
            emptyText.setCharacterSize(m_charSize);
            emptyText.setFillColor(sf::Color(120, 100, 140));
            emptyText.setPosition({ boxX + m_paddingX, boxY + m_paddingY + 30.f });
            window.draw(emptyText);
        }
        else {
            for (size_t i = 0; i < m_items.size(); ++i) {
                sf::Text itemText(font);
                std::string name = (m_items[i] == ItemType::Heal) ? "Bandages (+3 HP)"
                    : "Sedative (-Fear)";

                if (static_cast<int>(i) == m_selectedIndex) {
                    itemText.setString("> " + name);
                    itemText.setFillColor(sf::Color(255, 255, 100));
                }
                else {
                    itemText.setString("  " + name);
                    itemText.setFillColor(sf::Color(200, 200, 200));
                }

                itemText.setCharacterSize(m_charSize);
                itemText.setPosition(
                    { boxX + m_paddingX,
                     boxY + m_paddingY + 35.f + (static_cast<float>(i) * m_itemSpacing) });
                window.draw(itemText);
            }
        }

        sf::Text hint(font);
        hint.setString("W/S-Navigate ENTER-Use F-Close");
        hint.setCharacterSize(m_charSize - 1);
        hint.setFillColor(sf::Color(140, 120, 160));
        float hintX = boxX + (m_boxWidth - hint.getLocalBounds().size.x) / 2.f;
        hint.setPosition({ hintX, boxY + m_boxHeight - m_paddingY - 5.f });
        window.draw(hint);
    }

}  // namespace somnia
