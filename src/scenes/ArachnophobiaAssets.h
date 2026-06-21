#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "nlohmann/json.hpp"

namespace somnia {
namespace assets {

struct ArachnophobiaData {
  std::string fontPath;
  std::string mapPath;
  std::string playerTexture;
  std::string spiderTexture;

  struct BossStrings {
    std::string name;
    std::string greeting;
    std::string defeatMsg;
    std::string hitMsg;
    std::string dodgeMsg;
    std::string talkMsg;
  } boss;

  struct UiStrings {
    std::string chestTitle;
    std::string lootHeal;
    std::string lootFear;
  } ui;

  bool loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "[ArachnoAssets] Failed to open " << filename << "\n";
      return false;
    }
    try {
      nlohmann::json j;
      file >> j;

      fontPath = j["paths"]["font_path"];
      mapPath = j["paths"]["map_path"];
      playerTexture = j["paths"]["player_texture"];
      spiderTexture = j["paths"]["spider_texture"];

      boss.name = j["boss"]["name"];
      boss.greeting = j["boss"]["greeting"];
      boss.defeatMsg = j["boss"]["defeat_msg"];
      boss.hitMsg = j["boss"]["hit_msg"];
      boss.dodgeMsg = j["boss"]["dodge_msg"];
      boss.talkMsg = j["boss"]["talk_msg"];

      ui.chestTitle = j["ui"]["chest_title"];
      ui.lootHeal = j["ui"]["loot_heal"];
      ui.lootFear = j["ui"]["loot_fear"];
      return true;
    } catch (const std::exception& e) {
      std::cerr << "[ArachnoAssets] JSON parse error: " << e.what() << "\n";
      return false;
    }
  }
};

}  // namespace assets
}  // namespace somnia
