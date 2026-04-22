#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class Enemy {
 public:
  Enemy(std::string name, int hp, int damage);
  virtual ~Enemy() = default;

  virtual void takeDamage(int amount);
  virtual int attack() const;

  bool isDead() const { return m_hp <= 0; }
  std::string getName() const { return m_name; }
  int getHp() const { return m_hp; }

 protected:
  std::string m_name;
  int m_hp;
  int m_damage;
};

class GiantSpider : public Enemy {
 public:
  GiantSpider() : Enemy(u8"Гигантский Паук", 60, 12) {}
};

class Developer : public Enemy {
 public:
  Developer() : Enemy(u8"Разработчик (BOSS)", 999, 10) {}  // Тестовый враг
};

#endif
