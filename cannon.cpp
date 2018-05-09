#include "cannon.h"

Cannon::Cannon(int32 x, int32 y)
  : Tower(Coord(x, y, 1), CANNON)
{

}

void Cannon::attack(Creature* creature)
{
  creature->damage(m_attack_damage);
}

void Cannon::upgrade()
{

}