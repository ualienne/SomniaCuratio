#include "Enemy.h"

Enemy::Enemy(std::string name, int hp, int damage)
    : m_name(name), m_hp(hp), m_damage(damage) {}

void Enemy::takeDamage(int amount) { m_hp -= amount; }

int Enemy::attack() const { return m_damage; }