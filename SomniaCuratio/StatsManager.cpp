#include "StatsManager.h"

#include <fstream>

PlayerStats StatsManager::load() {
  std::ifstream file("stats.txt");
  PlayerStats ps;
  if (file.is_open()) {
    file >> ps.name >> ps.hp >> ps.maxHp >> ps.damage >> ps.lucidity;
    file.close();
  } else {
    ps = {"Adam", 100, 100, 20, 100};
  }
  return ps;
}

void StatsManager::save(const PlayerStats& stats) {
  std::ofstream file("stats.txt");
  if (file.is_open()) {
    file << stats.name << "\n"
         << stats.hp << "\n"
         << stats.maxHp << "\n"
         << stats.damage << "\n"
         << stats.lucidity;
    file.close();
  }
}