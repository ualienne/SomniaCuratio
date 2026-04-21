#ifndef INVENTORY_H
#define INVENTORY_H

#include <SFML/Graphics.hpp>
#include <fstream>
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
  void removeItem(const std::string& name);  // Удаление (использование)

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