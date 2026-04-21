#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Npc {
public:

    Npc(const std::string& name, const std::string& texturePath);
    
    bool load();
    void setPosition(sf::Vector2f pos);
    void draw(sf::RenderWindow& window);
    void addReplica(const std::string& text);
    std::string getRandomReplica();

    
    std::string getName() const { return m_name; }
    sf::Vector2f getPosition() const {
      return m_sprite.getPosition();
    }

   private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    std::string m_name;
    std::string m_texturePath;
    std::vector<std::string> m_replicas;
};

#endif