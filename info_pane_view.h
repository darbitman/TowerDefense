//**************************************************************************************************//
// info_pane_view.h                                                                                 //
//                                                                                                  //
// Definition: Header file for CreatureInfoView class                                               //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //
// 5/24/2012 - This class is now a friend to InputDeviceHandler::processMouseInput()                //
//           - getProperties() removed.                                                             //
//                                                                                                  //
// Notes: This class is responsible for drawing the info pane window located above the status view  //
//        window. It is primarily used for outputting strings that represents possible commands for //
//        the game.                                                                                 //
//**************************************************************************************************//

#ifndef INFO_PANE_VIEW_H
#define INFO_PANE_VIEW_H

#pragma once

class GameState;

#include "view.h"
#include "bounding_box_helper.h"
#include "InputDeviceHandler.h"

// These constants are this view's position and dimensions (in percent) relative to the console window's dimensions.
// They allow dynamic resizing of this view's bounding box. 
const double kInfoPaneViewWidthBase		=	4;	// This view's X coordinate is located ~approx. 4% from the console window's left border.
const double kInfoPaneViewHeightBase	=	68; // This view's Y coordinate is located ~approx. 68% from the console window's top border.
const double kInfoPaneViewHeight		=	17; // This view's height takes up ~approx. 17% of the entire console window.
const double kInfoPaneViewWidth			=	73; // This view's width takes up ~approx. 73% of the entire console window.

class InfoPaneView :
	public View
{
	public:
		InfoPaneView();
		~InfoPaneView();
		InfoPaneView(GameState *);

		virtual void draw();
		virtual void draw(WORD);

		void clearView();
		void addMessage(string);
		void clear_messages();

	private:
		void init(GameState *);

		// Disable default copy constructor and assignment operator.
		InfoPaneView(const InfoPaneView &);
		InfoPaneView & operator=(const InfoPaneView &);

		// Allow processMouseInput function access to mItemTupleVector.
		friend WORD InputDeviceHandler::processMouseInput(WORD);

	private:
		WORD							wWhite, wRed,		// Font foreground/background color properties.
										wWhiteOnBlack,
										wBlackOnGray;

		int								top,				// Item in the deque that will be the first string to be drawn.
										bottom,				// Item in the deque that will be the last string to be drawn.
										nViewableMessages;	// Number of messages that can fit in the info pane view (dynamically calculated).
		static const std::size_t		kMaxItems = 100;	// Max number of messages.
		std::string						str;				// Temporary string storage.
		
		ItemTupleVectorIter				prev;				// Previous item in the vector that was selected.
		bool							initialized,
										execonce;

		ItemTupleVector					mEditControlCommands;	// Holds control commands tuple objects.
		ItemTupleVector					mMapEditorCommands;	// Holds map editor commands tuple objects.
		ItemTupleVector					mBuildCommands;		// Holds build commands tuple objects.

		GameState						*gs;				// Instance of GameState class.
		BoundingBoxHelper				*bb;				// Instance of BoundingBoxHelper class.

		deque<std::string>				messages;			// String container where the scrollbar properties will be stored.
															// Using dequeue instead of queue so we can random access the strings.
		deque<std::string>::iterator	it;					// Iterator to string deque.
};

#endif
