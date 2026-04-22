#ifndef STATSMANAGER_H
#define STATSMANAGER_H

#include <string>

struct PlayerStats {
  std::string name;
  int hp;
  int maxHp;
  int damage;
  int lucidity;  // Новый параметр
};

class StatsManager {
 public:
  static PlayerStats load();
  static void save(const PlayerStats& stats);
};

#endif