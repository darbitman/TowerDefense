//**************************************************************************************************//
//  GameOverView.h                                                                                  //
//                                                                                                  //
//  Definition: Header file for CGameOverView class.                                                //
//  Version 0.2                                                                                     //
//                                                                                                  //
//  Changelog:                                                                                      //
//  5/23/2012 - Re-implemented.                                                                     //
//  5/24/2012 - getProperties() removed.                                                            //
//**************************************************************************************************//
#ifndef GAMEOVERVIEW_H
#define GAMEOVERVIEW_H

class GameState;

#include "view.h"
#include "bounding_box_helper.h"
#include "Logger\Logger.h"
#include "InputDeviceHandler.h"

class GameOverView : public View
{
	public:
		GameOverView();
		~GameOverView();
		GameOverView(GameState *);

		//******************************************************************************//
		// Function Name: draw().                                                       //
		// Description: Local implementation of view class's virtual function.          //
		//******************************************************************************//
		virtual void draw();
		virtual void draw(WORD);

		void setStatus(WORD);

	private:
		void init(GameState *);

		// Disable default copy constructor and assignment operator.
		GameOverView(const GameOverView &);
		GameOverView & operator=(const GameOverView &);

		// Allow processMouseInput function access to mItemTupleVector.
		friend WORD InputDeviceHandler::processMouseInput(WORD);

	private:
		WORD				wStatus;
		WORD				wRed, wWhite,		// Holds possible colors for the current string.	
							wWhiteOnBlack,
							wBlackOnGray;

		GameState			*gs;
		BoundingBoxHelper	*bb;

		std::string			str;
};

#endif
