#include "laser.h"

Laser::Laser(int32 x, int32 y)
  : Tower(Coord(x, y, 1), LASER)
{

}

void Laser::attack(Creature* creature)
{
  creature->damage(m_attack_damage);
}

void Laser::upgrade()
{

}