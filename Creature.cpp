#include "creature.h"

Creature::Creature(int32 x, int32 y, CreatureType ct)
  : Model(0, Coord(x, y, 1), 0X72)
{
  m_creature_type = ct;
  m_previous = m_xy;
  m_img      = CREATURE_STATS[ct].img;
  m_health   = m_max_health = CREATURE_STATS[ct].health;
  m_speed    = CREATURE_STATS[ct].speed;
  m_weakness = CREATURE_STATS[ct].weakness;
  m_strength = CREATURE_STATS[ct].strength;
  m_reward   = CREATURE_STATS[ct].reward;
}

int32 Creature::get_speed() const
{
  return m_speed;
}

int32 Creature::get_health() const
{
  return m_health;
}

DamageType Creature::get_weakness() const
{
  return m_weakness;
}

DamageType Creature::get_strength() const
{
  return m_strength;
}

uint32 Creature::get_reward() const
{
  return m_reward;
}

Coord Creature::get_previous_coord() const
{
  return m_previous;
}

void Creature::slow(int32 speed)
{
  m_speed -= speed;
}

void Creature::damage(int32 hp)
{
  m_health -= hp;
}

void Creature::set_previous_coord(Coord pos)
{
  m_previous.x = pos.x;
  m_previous.y = pos.y;
}

bool Creature::is_backtrack(const Coord& pos) const
{
  return (m_previous.x == pos.x && m_previous.y == pos.y);
}

bool Creature::is_dead() const
{
  return m_health <= 0;
}

void Creature::set_color()
{
  if (m_health / m_max_health < .33)
  {
    Model::set_color(0x74);
  }
  else if (m_health / m_max_health < .66)
  {
    Model::set_color(0x7E);
  }
}
