#include "tower.h"

Tower::Tower(Coord pos, TowerType towerType)
  : Model(TOWER_STATS[towerType].img, pos, 0X07)
{
  m_img           = TOWER_STATS[towerType].img;
  m_radius        = TOWER_STATS[towerType].radius;
  m_cost          = TOWER_STATS[towerType].cost;
  m_attack_damage = TOWER_STATS[towerType].attack_damage;
  m_attack_speed  = TOWER_STATS[towerType].attack_speed;
  m_attack_type   = TOWER_STATS[towerType].attack_type;
}

int32 Tower::get_radius() const
{
  return m_radius;
}

uint32 Tower::get_cost() const
{
  return m_cost;
}

uint32 Tower::sell()
{
  return m_cost / 2;
}