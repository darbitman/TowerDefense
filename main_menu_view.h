//**************************************************************************************************//
// info_pane_view.h                                                                                 //
//                                                                                                  //
// Definition: Header file for MainMenuView class.                                                  //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //
// 5/24/2012 - This class is now a friend to InputDeviceHandler::processMouseInput()                //
//           - getProperties() removed.                                                             //
//                                                                                                  //
// Notes: This class is responsible for displaying strings which are bounded to arbitrary commands. //
//        This class is also the first one to be drawn after the applications startup               //
//        initialzation. The strings in this class contains properties that allow them to be        //
//        interactive, and will respond to keyboard, mouse click/hover events.                      //
//**************************************************************************************************//

#ifndef MAIN_MENU_VIEW_H
#define MAIN_MENU_VIEW_H

#pragma once

#include "view.h"
#include "Config.h"
#include "td_console.h"
#include "game_state.h"
#include "InputDeviceHandler.h"

class GameState;

class MainMenuView :
	public View
{
	public:
		MainMenuView();
		~MainMenuView();
		MainMenuView(GameState *);
		
		virtual void draw();
		virtual void draw(WORD);

	private:
		void init(GameState *);

		// Disable default copy constructor and assignment operator.
		MainMenuView(const MainMenuView &);
		MainMenuView & operator=(const MainMenuView &);

		// Allow processMouseInput function access to mItemTupleVector.
		friend WORD InputDeviceHandler::processMouseInput(WORD);

	private:
		GameState		*gs;
		TDConsole		*console;			// Instance of TDConsole class.
		WORD			wRed, wWhite,		// Holds possible colors for the current string.
						wWhiteOnBlack,
						wBlackOnGray;
		std::string		str;				// Temporary string holder.
		//ItemTupleVectorIter prev;			// Previous item in the vector that was selected.
		//bool			initialized;

};

#endif
