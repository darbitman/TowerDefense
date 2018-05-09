#pragma once
#include "model.h"

enum CreatureType
{
  GOBLIN, // avg speed  | avg hp
  ORC,    // slow speed | high hp
  TROLL,  // fast speed | low hp
  TAUREN, // avg speed  | high hp
  CREATURE_TYPE_LAST
};

typedef struct
{
  int8 img;
  int32 health;
  int32 speed;
  DamageType weakness;
  DamageType strength;
  uint32 reward;
} CreatureStats;

// Simulate a database. Stats are in order of CreatureType.
// img - health - speed - weakness - strength - reward
static CreatureStats CREATURE_STATS[CREATURE_TYPE_LAST] =
{
  { (int8)157, 5,  2, BLUDGEON,  ELEMENTAL, 20 }, // Goblin
  { (int8)233, 15, 1, PIERCE,    BLUDGEON,  40 }, // Orc
  { (int8)231, 5,  3, ELEMENTAL, PIERCE,    30 }, // Troll
  { (int8)234, 15, 2, SLASH,     BLUDGEON,  50 }  // Tauren
};

class Creature : public Model
{
public:
  explicit Creature(int32 x, int32 y, CreatureType ct);
	int32  get_speed() const;
  int32  get_health() const;
  DamageType get_weakness() const;
  DamageType get_strength() const;
  uint32 get_reward() const;
  Coord  get_previous_coord() const;

	void slow(int32 speed);
  void damage(int32 hp);
  void set_previous_coord(Coord pos);
  void set_color();

  bool is_backtrack(const Coord& pos) const;
  bool is_dead() const;

  Coord converge;   //coord before the path converges so that creatures can't move in that direction

private:
  int32  m_speed;
  int32  m_health;
  double m_max_health;
  uint32 m_reward;
  Coord  m_previous; // Make sure we don't back track.
  DamageType   m_weakness;
  DamageType   m_strength;
  CreatureType m_creature_type;
};