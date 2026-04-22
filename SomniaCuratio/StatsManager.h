#ifndef STATSMANAGER_H
#define STATSMANAGER_H

#include <fstream>
#include <string>

struct PlayerStats {
  int hp;
  int maxHp;
  int damage;
  std::string name = "Adam";  // Имя для панели боя
};

class StatsManager {
 public:
  static PlayerStats load() {
    PlayerStats stats{100, 100, 20};  // Значения по умолчанию
    std::ifstream file("stats.txt");
    if (file.is_open()) {
      file >> stats.hp >> stats.maxHp >> stats.damage;
    }
    return stats;
  }

  static void save(const PlayerStats& stats) {
    std::ofstream file("stats.txt");
    if (file.is_open()) {
      file << stats.hp << " " << stats.maxHp << " " << stats.damage;
    }
  }
};

#endif