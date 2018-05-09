#pragma once
#include "Config.h"
//#include "view.h"
#include "player.h"
#include "cursor.h"
#include "map.h"
#include "creature.h"
#include "ViewManager.h"
#include "CreatureManager.h"
#include "TowerManager.h"
#include "Game.h"
#include "td_console.h"
#include "info_pane_view.h"
#include "tower_info_view.h"
#include "status_view.h"
#include "tile_info_view.h"
#include "map_view.h"
#include "Filesystem.h"


class Game;
class CreatureManager;
class InfoPaneView;
class TowerInfoView;
class StatusView;
class TowerManager;
class TileInfoView;
class MapView;
class CreatureInfoView;

class GameState
{
public:
  GameState(Game* _game);
  ~GameState();

  CreatureManager* creatureMgr() const;
  TowerManager* towerMgr() const;
  Game* game() const;
  void switchMap( int, int );
  void switchMap( string );

  ViewManager views;
  Player *player;
  game_state gs;
  Cursor *cursor;
  Map *map;
  TDConsole *console;
  InfoPaneView *info_pane_view;
  TowerInfoView *tower_info_view;
  StatusView *status_view;
  TileInfoView *tile_info_view;
  MapView *map_view;
  friend class CreatureInfoView;
  Filesystem * filesystem;

private:
  CreatureManager* m_creature_mgr;
  TowerManager* m_tower_mgr;
  Game* m_game;
};

