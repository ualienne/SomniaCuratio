#include "Inventory.h"

Inventory::Inventory() {}

void Inventory::loadFromFile(const std::string& filename) {
  m_items.clear();
  std::ifstream file(filename);
  if (!file.is_open()) return;

  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty() && (unsigned char)line[0] == 0xEF) {
      line.erase(0, 3);
    }

    std::stringstream ss(line);
    std::string name, countStr, stats, cat;

    std::getline(ss, name, ';');
    std::getline(ss, countStr, ';');
    std::getline(ss, stats, ';');
    std::getline(ss, cat, ';');

    if (!name.empty()) {
      m_items.push_back({name, std::stoi(countStr), stats, cat});
    }
  }
}
void Inventory::saveToFile(const std::string& filename) {
  std::ofstream file(filename);
  for (const auto& item : m_items) {
    file << item.name << ";" << item.count << ";" << item.stats << ";"
         << item.category << "\n";
  }
}

void Inventory::addItem(std::string name, int count, std::string stats,
                        std::string category) {
  int currentConsumables = 0;
  for (const auto& i : m_items)
    if (i.category == "Consumables") currentConsumables++;

  if (category == "Consumables" && currentConsumables >= MAX_CONSUMABLES)
    return;

  m_items.push_back({name, count, stats, category});
  saveToFile("inventory.txt");
}

void Inventory::draw(sf::RenderWindow& window, const sf::Font& font) {
  if (!m_isVisible) return;
  sf::RectangleShape bg({300.f, 400.f});
  bg.setFillColor(sf::Color::Black);
  bg.setOutlineColor(sf::Color::White);
  bg.setOutlineThickness(2.f);
  bg.setPosition({50.f, 50.f});
  window.draw(bg);

  // Используем u8 для заголовка
  std::string header = u8"--- ИНВЕНТАРЬ ---\n\n";

  std::string itemsContent = "";
  for (const auto& item : m_items) {
    itemsContent += item.name + " x" + std::to_string(item.count) + "\n";
    itemsContent += "  [" + item.stats + "]\n\n";
  }

  // Соединяем заголовок и содержимое
  std::string fullString = header + itemsContent;

  // Преобразуем всю итоговую UTF-8 строку в понятный SFML формат - иначе я при
  // тесте сталкиваюсь с одной точкой
  sf::Text text(font);
  text.setString(sf::String::fromUtf8(fullString.begin(), fullString.end()));
  text.setCharacterSize(22);
  text.setFillColor(sf::Color::White);
  text.setPosition({75.f, 80.f});

  window.draw(text);
}
