#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

namespace somnia {

class NPC : public sf::Drawable {
 public:
  NPC() = default;

  NPC(const NPC&) = delete;
  NPC& operator=(const NPC&) = delete;
  NPC(NPC&&) = default;
  NPC& operator=(NPC&&) = default;

  bool loadTexture(const std::string& pngPath);

  void placeAt(sf::Vector2f worldPos);

  void setScale(float scale);

  void setName(const std::string& name) { m_name = name; }
  void setGreeting(const std::string& greeting) { m_greeting = greeting; }

  std::string name() const { return m_name; }
  std::string greeting() const { return m_greeting; }

  sf::Vector2f position() const {
    return m_sprite ? m_sprite->getPosition() : sf::Vector2f{0.f, 0.f};
  }

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  std::shared_ptr<sf::Texture> m_texture;
  std::unique_ptr<sf::Sprite> m_sprite;

  std::string m_name;
  std::string m_greeting;
};

}  // namespace somnia
