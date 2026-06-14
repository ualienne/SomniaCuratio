#pragma once
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace somnia {
namespace assets {

struct ArachnophobiaData {
  std::string fontPath;
  std::string mapPath;
  std::string playerTexture;
  std::string spiderTexture;

  std::string chestTitle;
  std::string chestHealMsg;
  std::string chestFearMsg;

  std::string bossName;
  std::string bossEngageMsg;
  std::string bossDeathMsg;
  std::string bossHitMsg;
  std::string bossDodgeMsg;
  std::string bossTalkMsg;

  std::string hudHintText;

  bool loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "[ArachnophobiaAssets] Failed to open " << filename << "\n";
      return false;
    }
    try {
      nlohmann::json j;
      file >> j;

      fontPath = j["paths"]["font_path"];
      mapPath = j["paths"]["map_path"];
      playerTexture = j["paths"]["player_texture"];
      spiderTexture = j["paths"]["spider_texture"];

      chestTitle = j["dialogues"]["chest_title"];
      chestHealMsg = j["dialogues"]["chest_heal_msg"];
      chestFearMsg = j["dialogues"]["chest_fear_msg"];
      bossName = j["dialogues"]["boss_name"];
      bossEngageMsg = j["dialogues"]["boss_engage_msg"];
      bossDeathMsg = j["dialogues"]["boss_death_msg"];
      bossHitMsg = j["dialogues"]["boss_hit_msg"];
      bossDodgeMsg = j["dialogues"]["boss_dodge_msg"];
      bossTalkMsg = j["dialogues"]["boss_talk_msg"];
      hudHintText = j["dialogues"]["hud_hint_text"];
      return true;
    } catch (const std::exception& e) {
      std::cerr << "[ArachnophobiaAssets] JSON parse error: " << e.what()
                << "\n";
      return false;
    }
  }
};

}  // namespace assets
}  // namespace somnia
