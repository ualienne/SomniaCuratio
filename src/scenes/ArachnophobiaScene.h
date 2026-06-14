#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/NightmareOutcome.h"
#include "entities/BossSpider.h"
#include "entities/Player.h"
#include "entities/Spider.h"
#include "scenes/Scene.h"
#include "ui/DialogueManager.h"
#include "world/Camera.h"
#include "world/TileMap.h"
#include "ui/Inventory.h"

namespace somnia {

    class ArachnophobiaScene : public Scene {
    public:
        using OutcomeCallback = std::function<void(NightmareOutcome)>;

        ArachnophobiaScene(sf::RenderWindow& window,
            const std::string& spawnPointName, OutcomeCallback onExit);

        void handleEvent(const sf::Event& event) override;
        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;

    private:
        enum class TurnState { PlayerTurn, MonstersTurn, CheckOutcomes };
        enum class BattleAction { None, Attack, Dodge, Talk };

        void checkOutcome();
        void renderHUD(sf::RenderWindow& window);
        void tryEngageBoss();
        void resolveBossAction();

        bool isTileNear(sf::Vector2i a, sf::Vector2i b, int distX, int distY) const;

        static constexpr int m_defaultSpawnTileX = 3;
        static constexpr int m_defaultSpawnTileY = 3;
        static constexpr size_t m_spidersReserveCount = 10;
        static constexpr int m_spiderLoopStart = 1;
        static constexpr int m_spiderLoopEnd = 8;
        static constexpr float m_spiderScale = 2.f;
        static constexpr float m_chestDividerX = 3.f;
        static constexpr float m_chestDividerY = 3.f;

        static constexpr int m_chestInteractDistX = 3;
        static constexpr int m_chestInteractDistY = 3;
        static constexpr int m_chestHealAmount = 3;
        static constexpr float m_chestFearReduceAmount = 8.f;
        static constexpr float m_fearTimerMinLimit = 0.f;

        static constexpr int m_playerAttackDistX = 3;
        static constexpr int m_playerAttackDistY = 3;
        static constexpr int m_playerAttackDamage = 3;

        static constexpr int m_bossDamageToBoss = 2;
        static constexpr int m_bossDamageToPlayer = 2;
        static constexpr float m_bossTalkFearReduce = 5.f;

        static constexpr int m_spiderMonsterTurnDamage = 1;
        static constexpr int m_spiderNearbyCheckDistX = 1;
        static constexpr int m_spiderNearbyCheckDistY = 1;
        static constexpr float m_fearRecoveryMultiplier = 0.5f;

        static constexpr unsigned int m_hudCharacterSizeLarge = 12u;
        static constexpr unsigned int m_hudCharacterSizeMedium = 10u;
        static constexpr unsigned int m_hudCharacterSizeSmall = 9u;

        static constexpr float m_hudTextPaddingX = 12.f;
        static constexpr float m_hudTextPaddingY = 12.f;
        static constexpr float m_hudHpTextPaddingY = 32.f;
        static constexpr float m_hudBossHpTextOffsetX = 70.f;
        static constexpr float m_hudHintPaddingBottom = 20.f;
        static constexpr float m_hudFearLabelPaddingY = 26.f;

        static constexpr float m_hudBarWidth = 180.f;
        static constexpr float m_hudBarHeight = 10.f;
        static constexpr float m_hudBarFillMaxPct = 1.f;
        static constexpr float m_hudBarOutlineThickness = 1.f;
        static constexpr float m_viewCenterDivider = 2.f;

        sf::RenderWindow& m_window;
        sf::Font m_font;
        bool m_fontLoaded = false;
        bool m_waitingForClose = false;

        TileMap m_map;
        Camera m_camera;
        Player m_player;

        std::string m_spawnPointName;
        OutcomeCallback m_onExit;

        std::vector<std::unique_ptr<Spider>> m_spiders;
        std::unique_ptr<BossSpider> m_boss;
        bool m_bossEngaged = false;
        BattleAction m_pendingAction = BattleAction::None;

        std::optional<Facing> m_nextDirection;
        TurnState m_currentTurn = TurnState::PlayerTurn;

        int m_spidersTotal = 0;
        int m_spidersKilled = 0;
        float m_fearTimer = 0.f;
        float m_fearMax = 30.f;
        float m_escapeTimer = 0.f;
        float m_escapeRequired = 5.f;
        bool m_finished = false;
        bool m_mapReady = false;

        bool m_hasPendingOutcome = false;
        NightmareOutcome m_pendingOutcome{};

        struct ChestData {
            sf::Vector2f position;
            bool opened = false;
            enum class LootType { Heal, FearReduce } loot;
        };
        std::vector<ChestData> m_chests;
        Inventory m_inventory;
    };

}  // namespace somnia