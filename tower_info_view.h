#pragma once
#include "view.h"
#include "game_state.h"
#include "bounding_box_helper.h"

class GameState;

class TowerInfoView :
	public View
{
	public:
		TowerInfoView( GameState * );
		~TowerInfoView();

		virtual void draw();
		virtual void draw(WORD);

		void selectionInc();
		int getSelection();

	private:
		GameState *gs;
		BoundingBoxHelper *bb;
		int selection;
};

