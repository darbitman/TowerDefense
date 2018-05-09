#include "game_state.h"

GameState::GameState(Game* game)
{
	m_game = game;
	player = new Player();
	gs = main_menu;
	cursor = new Cursor();
	map = new Map("maps/basic1.tdm");
	m_creature_mgr = new CreatureManager(map, this, player);
	m_tower_mgr = new TowerManager(map, player, this);
	console = new TDConsole();
	info_pane_view = NULL;
	tower_info_view = NULL;
	status_view = new StatusView(this);;
	map_view = new MapView(this);
	filesystem = new Filesystem();
}

GameState::~GameState()
{
  delete player;
  delete cursor;
  delete map;
  delete m_creature_mgr;
  delete m_tower_mgr;
  delete console;
  //delete status_view; // automatically deleted at game over.
  //delete map_view;    // automatically deleted when a new map is created.
  delete filesystem;
}

CreatureManager* GameState::creatureMgr() const
{
  return m_creature_mgr;
}

TowerManager* GameState::towerMgr() const
{
  return m_tower_mgr;
}

Game* GameState::game() const
{
  return m_game;
}

void GameState::switchMap( int width = 15, int height = 15) {
	delete map;
	map = new Map( width, height );
}

void GameState::switchMap( string new_map_name ){
	delete map;
	map = new Map( new_map_name );
	map_view->update( this );
}