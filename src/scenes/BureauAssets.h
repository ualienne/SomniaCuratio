#pragma once
#include <string>

namespace somnia {
namespace assets {

inline const std::string FontPath = "assets/fonts/PressStart2P.ttf";
inline const std::string MapPath = "assets/maps/bureau.tmx";
inline const std::string PlayerTexture = "assets/sprites/characters/agent.png";

inline const std::string NpcVossTexture = "assets/sprites/characters/voss.png";
inline const std::string NpcVossName = "Dr. Emil Voss";
inline const std::string NpcVossGreeting =
    "Ты как бледная поганка. Кошмарики задрали?";

inline const std::string NpcKiraTexture = "assets/sprites/characters/kira.png";
inline const std::string NpcKiraName = "Kira";
inline const std::string NpcKiraGreeting = "Клац-клац..";

inline const std::string NpcHayesTexture =
    "assets/sprites/characters/hayes.png";
inline const std::string NpcHayesName = "Hayes";
inline const std::string NpcHayesGreeting = "Не трогай мои файлы.";

inline const std::string HelpText = "WASD - move   E - interact   ESC - quit";
inline const std::string SysTitle = "Система";
inline const std::string MsgTherapy = "Кошмар побеждён.\nСтрах отступил.";
inline const std::string MsgFailure =
    "Кошмар поглотил тебя...\nНо ты проснулась.";
inline const std::string MsgNeutral = "Ты сбежала из кошмара.\nСтрах остался.";

}  // namespace assets
}  // namespace somnia
