#include "CombatManager.h"

#include "StatsManager.h"
void CombatManager::startCombat(std::unique_ptr<Enemy> enemy) {
  m_currentEnemy = std::move(enemy);
  m_state = CombatState::PLAYER_TURN;
  m_lastActionMessage = L"";
}

void CombatManager::playerAttack() {
  if (m_state != CombatState::PLAYER_TURN) return;
  PlayerStats ps = StatsManager::load();
  m_currentEnemy->takeDamage(ps.damage);
  if (m_currentEnemy->isDead())
    m_state = CombatState::WIN;
  else
    m_state = CombatState::ENEMY_TURN;
}

void CombatManager::playerDefend() {
  m_isDefending = true;
  m_state = CombatState::ENEMY_TURN;
}

void CombatManager::enemyTurn() {
  if (m_state != CombatState::ENEMY_TURN) return;
  PlayerStats ps = StatsManager::load();
  int dmg = m_currentEnemy->attack();
  if (m_isDefending) {
    dmg /= 2;
    m_isDefending = false;
  }

  ps.hp -= dmg;
  StatsManager::save(ps);

  m_lastActionMessage = L"���� ����� " + std::to_wstring(dmg) + L" �����!";

  if (ps.hp <= 0)
    m_state = CombatState::LOSE;
  else
    m_state = CombatState::PLAYER_TURN;
}