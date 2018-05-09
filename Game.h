#ifndef GAME_H
#define GAME_H

#pragma once

#define SAVE_MAP	0
#define LOAD_MAP	1

#include "Config.h"
#include "main_menu_view.h"
#include "map_view.h"
#include "player_info_view.h"
#include "build_view.h"
#include "GameOverView.h"
#include "game_state.h"
#include "cannon.h"
#include "creature.h"
#include "sniper.h"
#include "laser.h"
#include "bounding_box.h"
#include "td_console.h"
#include "tower_info_view.h"
#include "info_pane_view.h"
#include "creature_info_view.h"
#include "status_view.h"
#include "tile_info_view.h"
#include "filename_view.h"
#include "AboutView.h"
#include "HelpView.h"
#include "InputDeviceHandler.h"

class GameState;

// The main controller for the overall game.
class Game {
	public:
		Game();
		~Game();

		// Starts the game. When run exits, the game will shutdown.
		void run();
		void change(bool val) { m_change = val; }
		void restart_timer(bool val) { m_restart_timer = val; }

	private:
		GameState *gs;
		InputDeviceHandler * inputdevice;
		Tower *t;

		void main_menu_init();
		void main_menu_run();
		void leave_main_menu();

		void map_editor_init();
		void map_editor_run();
		void tile_selector( char );

		void play_init();

		void edit_mode_run();
		void switch_to_build();
		void switch_to_edit();
		void build_mode_run();

		void show_build_commands();       //update info_pane
		void show_control_commands();     //update info_pane
		void show_chosen_tower();         //update status_view
		void show_map_editor_commands();

		void run_mode_init();
		void run_mode_run( uint32 );
		void exit_run_mode();

		void draw_all_views();
		void update_cursor( Coord, Coord );

		void setCursorImage();

		void loadFileDialogBox(SHORT);
		void loadAboutView();
		void loadHelpView();

	private:
		bool m_change;
		bool m_restart_timer;
		bool run_once;
		uint32 lives_count;
};

#endif
