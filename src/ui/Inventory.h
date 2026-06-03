#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <string>

namespace somnia {

  class Player;

  class Inventory {
  public:
    enum class ItemType {
      Heal,
      FearReduce
    };

    Inventory() = default;

    void addItem(ItemType item);

    void handleEvent(const sf::Event& event, Player& player, float& fearTimer, float fearTimerMinLimit, float chestFearReduceAmount);
    void render(sf::RenderWindow& window, const sf::Font& font) const;

    bool isOpen() const { return m_isOpen; }
    void toggle() { m_isOpen = !m_isOpen; }

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

  private:
    std::vector<ItemType> m_items;
    bool m_isOpen = false;
    int m_selectedIndex = 0;
    static constexpr unsigned int m_charSize = 10u;
    static constexpr float m_boxWidth = 360.f;
    static constexpr float m_boxHeight = 180.f;
    static constexpr float m_paddingX = 15.f;
    static constexpr float m_paddingY = 15.f;
    static constexpr float m_itemSpacing = 16.f;
    static constexpr int m_healAmount = 3;
  };

} // namespace somnia