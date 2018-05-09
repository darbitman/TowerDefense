#pragma once

#include "Config.h"

class Player
{
public:
  Player();
  
  uint32 get_money() const;
  uint32 get_lives() const;
  uint32 get_score() const;
  bool is_alive() const;
  
  void set_money(uint32);
  void add_money(int32);
  void add_score( uint32 );
  void remove_life();

private:
  uint32 m_money;
  uint32 m_lives;
  uint32 m_score;
};

