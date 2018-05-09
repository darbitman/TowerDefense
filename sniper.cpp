#include "sniper.h"

Sniper::Sniper(int32 x, int32 y)
  : Tower(Coord(x, y, 1), SNIPER)
{

}

void Sniper::attack(Creature* creature)
{
  creature->damage(m_attack_damage);
}

void Sniper::upgrade()
{

}