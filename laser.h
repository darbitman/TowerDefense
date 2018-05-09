#pragma once
#include "tower.h"

class Laser : public Tower
{
public:
  explicit Laser(int32 x, int32 y);

  void attack(Creature* creature);
  void upgrade();
};
