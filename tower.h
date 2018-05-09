#pragma once
#include "creature.h"

class Creature;

enum TowerType
{
  CANNON,
  LASER,
  SNIPER,
  TOWER_TYPE_LAST
};

typedef struct 
{
  int8   img;
  int32  radius;
  uint32 cost;
  int32  attack_damage;
  int32  attack_speed;
  DamageType attack_type;
} TowerStats;

// Simulate a database. Stats are in order of CreatureType.
// image - radius - cost - damage - speed - type
static TowerStats TOWER_STATS[TOWER_TYPE_LAST] = 
{
  { (int8)227, 1, 50,  1, 1, BLUDGEON  }, // Cannon
  { (int8)248, 3, 100, 4, 2, ELEMENTAL }, // Laser
  { '+',       5,  75, 2, 3, PIERCE    }, // Sniper
};

class Tower : public Model
{
public:
  int32  get_radius() const;
  uint32 get_cost() const;
  uint32 sell();

  virtual void attack(Creature* creature) = 0;
  virtual void upgrade() = 0;

protected:
  explicit Tower(Coord pos, TowerType tt);

  int32  m_radius;
  uint32 m_cost;
  uint32 m_rank;
  int32  m_attack_damage;
  int32  m_attack_speed;      //rounds to wait before shooting
  DamageType m_attack_type;
};
