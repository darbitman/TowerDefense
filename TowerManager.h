#pragma once

#include "Config.h"
#include "tower.h"
#include "player.h"
#include "map.h"
#include "creature.h"
#include "game_state.h"

class GameState;

class TowerManager
{
public:
	TowerManager(Map* map, Player* player, GameState *_gs);
  void add_tower(Tower* tower);
  void remove_tower_at(const Coord& xy);
  void reset();
  void range_check();

private:
  bool _is_in_range(Tower* tower, const Coord& xy);
  GameState *gs;
  Map* m_map;
  Player* m_player;
	list<Tower*> m_towers;
};