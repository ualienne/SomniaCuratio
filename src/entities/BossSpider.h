#pragma once

#include <SFML/Graphics.hpp>

namespace somnia {

    class BossSpider : public sf::Drawable {
    public:
        BossSpider() = default;
        BossSpider(const BossSpider&) = delete;
        BossSpider& operator=(const BossSpider&) = delete;

        void placeAt(sf::Vector2f worldPos) { m_pos = worldPos; }

        bool takeDamage(int dmg = 1) {
            m_hp -= dmg;
            if (m_hp < m_minHp) m_hp = m_minHp;
            return isDead();
        }

        bool isDead() const { return m_hp <= m_minHp; }
        int hp() const { return m_hp; }
        int maxHp() const { return m_maxHp; }
        sf::Vector2f position() const { return m_pos; }
        float interactRadius() const { return m_interactRadius; }

    private:
        void draw(sf::RenderTarget&, sf::RenderStates) const override {}

        static constexpr int m_minHp = 0;
        static constexpr float m_interactRadius = 300.f;

        sf::Vector2f m_pos{ 0.f, 0.f };
        int m_hp = 6;
        int m_maxHp = 6;
    };

}  // namespace somnia
