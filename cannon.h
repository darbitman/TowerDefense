#pragma once
#include "tower.h"

class Cannon : public Tower
{
public:
  explicit Cannon(int32 x, int32 y);

  void attack(Creature* creature);
  void upgrade();
};
