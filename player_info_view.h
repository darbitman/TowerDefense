#pragma once
#include "view.h"
#include "Config.h"
#include "player.h"
#include "game_state.h"
#include "bounding_box_helper.h"

class GameState;

class PlayerInfoView :
	public View
{
	public:
		PlayerInfoView( GameState * );
		~PlayerInfoView();
		virtual void draw();
		virtual void draw(WORD);

	private:
		GameState			*gs;
		BoundingBoxHelper	*bb;
};

