#pragma once
#include "tower.h"

class Sniper : public Tower
{
public:
  explicit Sniper(int32 x, int32 y);

  void attack(Creature* creature);
  void upgrade();
};
