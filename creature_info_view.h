//**************************************************************************************************//
// creature_info_view.h                                                                             //
//                                                                                                  //
// Definition: Header file for CreatureInfoView class                                               //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //
//                                                                                                  //
// Notes: This class is responsible for drawing the creature information located at the lower-right //
//        of the console window.                                                                    //
//**************************************************************************************************//

#ifndef CREATURE_INFO_VIEW_H
#define CREATURE_INFO_VIEW_H

#pragma once

#include "view.h"
#include "bounding_box_helper.h"
#include "game_state.h"

// These constants are this view's position and dimensions (in percent) relative to the console window's dimensions.
// They allow dynamic resizing of this view's bounding box. 
const double kCreatureInfoViewWidthBase		=	78; // This view's X coordinate is located ~approx. 78% from the console window's left border.
const double kCreatureInfoViewHeightBase	=	68; // This view's Y coordinate is located ~approx. 68% from the console window's top border.
const double kCreatureInfoViewHeight		=	31; // This view's height takes up ~approx. 31% of the entire console window.
const double kCreatureInfoViewWidth			=	21; // This view's width takes up ~approx. 21% of the entire console window. 

class CreatureInfoView :
	public View
{
public:
	CreatureInfoView();
	~CreatureInfoView();
	CreatureInfoView( GameState * );

	virtual void draw();
	virtual void draw(WORD);

	private:
		void init(GameState *);

		// Disable default copy constructor and assignment operator.
		CreatureInfoView(const CreatureInfoView &);
		CreatureInfoView & operator=(const CreatureInfoView &);

	private:
		GameState			*gs;					// Instance of GameState class.
		BoundingBoxHelper	*bb;					// Instance of BoundingBoxHelper class.
};

#endif