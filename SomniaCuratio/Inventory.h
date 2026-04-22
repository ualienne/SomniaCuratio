#ifndef INVENTORY_H
#define INVENTORY_H

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Item {
  std::string name;
  int count;
  std::string stats;
  std::string category;
};

class Inventory {
 public:
  Inventory();
  void loadFromFile(const std::string& filename);
  void saveToFile(const std::string& filename);

  void addItem(std::string name, int count, std::string stats,
               std::string category);
  bool hasItem(const std::string& itemName) const {
    for (const auto& item : m_items) {
      if (item.name == itemName)
        return true;  // Сравниваем именно поле name объекта
    }
    return false;
  }

  void removeItem(const std::string& itemName) {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
      if (it->name == itemName) {
        m_items.erase(it);
        break;
      }
    }
  }

  void toggle() { m_isVisible = !m_isVisible; }
  void draw(sf::RenderWindow& window, const sf::Font& font);

 private:
  std::vector<Item> m_items;
  bool m_isVisible = false;

  // Лимиты по категориям
  const int MAX_CONSUMABLES = 10;
  const int MAX_ACCESSORY = 2;
};

#endif
