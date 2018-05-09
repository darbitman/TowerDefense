#pragma once
#include "view.h"
#include "map.h"
#include "model.h"
#include "game_state.h"
#include "bounding_box_helper.h"

class GameState;

class MapView :
	public View
{
	public:
		MapView(void);

		MapView( GameState* );
		~MapView();

		virtual void update( GameState* );

		virtual void draw();
		virtual void draw(WORD);

		Map * get_map();

	private :
		Map					*map;
		GameState			*gs;
		TDConsole			*console;
		BoundingBoxHelper	*bb;
};