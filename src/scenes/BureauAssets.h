#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace somnia {
    namespace assets {
        namespace bureau {

            struct NpcConfig {
                std::string texture;
                std::string name;
                std::string greeting;
            };

            struct BureauData {
                std::string fontPath;
                std::string mapPath;
                std::string playerTexture;

                NpcConfig voss;
                NpcConfig kira;
                NpcConfig hayes;

                std::string helpText;
                std::string sysTitle;
                std::string msgTherapy;
                std::string msgFailure;
                std::string msgNeutral;

                bool loadFromFile(const std::string& filename) {
                    std::ifstream file(filename);
                    if (!file.is_open()) {
                        std::cerr << "[BureauAssets] Failed to open " << filename << "\n";
                        return false;
                    }
                    try {
                        nlohmann::json j;
                        file >> j;

                        fontPath = j["paths"]["font_path"].get<std::string>();
                        mapPath = j["paths"]["map_path"].get<std::string>();
                        playerTexture = j["paths"]["player_texture"].get<std::string>();

                        auto loadNpc = [](const nlohmann::json& node) {
                            return NpcConfig{
                                node["texture"].get<std::string>(),
                                node["name"].get<std::string>(),
                                node["greeting"].get<std::string>()
                            };
                            };

                        voss = loadNpc(j["npcs"]["voss"]);
                        kira = loadNpc(j["npcs"]["kira"]);
                        hayes = loadNpc(j["npcs"]["hayes"]);

                        helpText = j["system"]["help_text"].get<std::string>();
                        sysTitle = j["system"]["sys_title"].get<std::string>();
                        msgTherapy = j["system"]["msg_therapy"].get<std::string>();
                        msgFailure = j["system"]["msg_failure"].get<std::string>();
                        msgNeutral = j["system"]["msg_neutral"].get<std::string>();

                        return true;
                    }
                    catch (const std::exception& e) {
                        std::cerr << "[BureauAssets] JSON parse error: " << e.what() << "\n";
                        return false;
                    }
                }
            };

        } // namespace bureau
    } // namespace assets
} // namespace somnia
