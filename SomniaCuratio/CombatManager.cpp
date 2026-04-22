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

  if (ps.hp <= 0) {
    ps.hp = 0;
    ps.lucidity -= 10;
    if (ps.lucidity < 0) ps.lucidity = 0;  
    m_state = CombatState::LOSE;
  } else {
    m_state = CombatState::PLAYER_TURN;
  }

  m_lastActionMessage = L"Враг нанес " + std::to_wstring(dmg) + L" урона!";
  StatsManager::save(ps);
}