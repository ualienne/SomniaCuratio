#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include <memory>
#include <string>

#include "Enemy.h"

enum class CombatState { NONE, PLAYER_TURN, ENEMY_TURN, WIN, LOSE };

class CombatManager {
 public:
  void startCombat(std::unique_ptr<Enemy> enemy);
  void playerAttack();
  void playerDefend();
  void enemyTurn();

  CombatState getState() const { return m_state; }
  Enemy* getEnemy() const { return m_currentEnemy.get(); }
  std::wstring getLastActionMessage() const { return m_lastActionMessage; }

 private:
  CombatState m_state = CombatState::NONE;
  std::unique_ptr<Enemy> m_currentEnemy = nullptr;
  bool m_isDefending = false;
  std::wstring m_lastActionMessage = L"";
};

#endif