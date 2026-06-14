#pragma once
#include <string>

namespace somnia {
	namespace assets {

		inline const std::string FontPath = "assets/fonts/PressStart2P.ttf";
		inline const std::string MapPath = "assets/maps/arachnophobia.tmx";
		inline const std::string PlayerTexture = "assets/sprites/characters/agent.png";
		inline const std::string SpiderTexture = "assets/sprites/enemies/spider.png";

		inline const std::string ChestTitle = "Сундук";
		inline const std::string ChestHealMsg = "Ты нашёл бинты.\nОни добавлены в инвентарь [F].";
		inline const std::string ChestFearMsg = "Успокоительное...\nДобавлено в инвентарь [F].";

		inline const std::string BossName = "Арахна";
		inline const std::string BossEngageMsg = "Ты осмелился войти в мои сети?\n[1] Атаковать\n[2] Уклониться\n[3] Говорить";
		inline const std::string BossDeathMsg = "Н-нет... невозможно...\nТы победил меня... страх отступает...\n[E] Закрыть";
		inline const std::string BossHitMsg = "Ты осмелился ударить меня?!\nHP: ";
		inline const std::string BossDodgeMsg = "Трус. Ты уклонился... на этот раз.\nНо паутина везде.\n[E] Закрыть";
		inline const std::string BossTalkMsg = "Слова... Любопытно.\nСтрах чуть отступил.\n[E] Закрыть";

		inline const std::string HUDHintText = "WASD-move  SPACE-attack  E-interact";

	} // namespace assets
} // namespace somnia