#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

namespace somnia {

    class TileMap;

    class Spider : public sf::Drawable {
    public:
        Spider() = default;
        Spider(const Spider&) = delete;
        Spider& operator=(const Spider&) = delete;

        bool loadTexture(const std::string& pngPath);

        void placeOnTile(sf::Vector2i tile, sf::Vector2u tileSize);
        void setScale(float scale);

        void stepTowardPlayer(sf::Vector2i playerTile, const TileMap& map);

        void update(float deltaTime, sf::Vector2f /*unused*/);

        bool takeDamage(int dmg = 1);

        bool isDead() const { return m_hp <= m_deadHpThreshold; }

        sf::Vector2i currentTile() const { return m_tile; }
        sf::Vector2f position() const {
            return m_sprite ? m_sprite->getPosition() : sf::Vector2f{};
        }
        float collisionRadius() const { return m_collisionRadius; }

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        static constexpr float m_spriteOriginX = 2.f;
        static constexpr float m_spriteOriginY = 2.f;
        static constexpr float m_tileHalfDivider = 2.f;

        static constexpr int m_playerNearDistanceX = 1;
        static constexpr int m_playerNearDistanceY = 1;
        static constexpr int m_gridStepPositive = 1;
        static constexpr int m_gridStepNegative = -1;
        static constexpr int m_deadHpThreshold = 0;

        static constexpr float m_animProgressMax = 1.f;
        static constexpr float m_collisionRadius = 14.f;

        std::shared_ptr<sf::Texture> m_texture;
        std::unique_ptr<sf::Sprite> m_sprite;

        int m_hp = 3;

        sf::Vector2i m_tile{ 0, 0 };
        sf::Vector2u m_tileSize{ 16, 16 };

        sf::Vector2f m_visualPos{ 0.f, 0.f };
        bool m_isAnimating = false;
        float m_animTimer = 0.f;

        static constexpr float kAnimDuration = 0.15f;
    };

}  // namespace somnia