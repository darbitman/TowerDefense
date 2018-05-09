//**************************************************************************************************//
// filename_view.h                                                                                  //
//                                                                                                  //
// Definition: Header file for FilenameView class                                                   //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //
//                                                                                                  //
// Notes: This class is responsible for drawing the file name input window that prompts for file    //
//        name. Located at the center of the console window.                                        //
//**************************************************************************************************//

#ifndef FILENAME_VIEW_H
#define FILENAME_VIEW_H

#pragma once

#include "view.h"
#include "game_state.h"
#include "bounding_box_helper.h"
#include "InputDeviceHandler.h"


typedef struct BoxDimensions
{
	SHORT baseX;
	SHORT baseY;
	SHORT width;
	SHORT height;
	bool single;
} BOX_DIMENSIONS;

class FilenameView :
	public View
{
	public:
		FilenameView();
		~FilenameView();
		FilenameView( GameState *, WORD );

		virtual void draw();
		virtual void draw(WORD);
		virtual void draw(std::string &);

		// Breaks MVC paradigm?
		std::string getFileNameSelected();

	private:
		void init(GameState *);
		void clearFileInputBox();

		// Disable default copy constructor and assignment operator.
		FilenameView(const FilenameView &);
		FilenameView & operator=(const FilenameView &);

		// Allow processMouseInput function access to mItemTupleVector.
		friend WORD InputDeviceHandler::processMouseInput(WORD);

	private:

		WORD				wWhite, wRed,
							wWhiteOnBlack,
							wBlackOnGray;

		WORD				wState;

		BOX_DIMENSIONS		bd1, bd2, bd3, bd4;
		std::string			str;
		SHORT				posX;

		ItemTupleVector		mMapFilesVector;

		// Original box properties the Filename box is going to write.
		CHAR_INFO			chiBuffer[1215]; // 45 x 27
		COORD				coordBufSize;
		COORD				coordBufCoord;
		SMALL_RECT			srctReadRect;

		GameState			*gs;			// Instance of GameState class.
		BoundingBoxHelper	*bb;			// Instance of BoundingBoxHelper class.
};

#endif
