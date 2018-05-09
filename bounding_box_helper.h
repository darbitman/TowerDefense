//**************************************************************************************************//
// bounding_box_helper.h                                                                            //
//                                                                                                  //
// Definition: Header file for BoundingBoxHelper class                                              //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //
//                                                                                                  //
// Notes: This class is responsible for drawing the actual bounding box around the console window.  //
//        Each character in the box is represented by an extended ascii code.                       //
//**************************************************************************************************//

#ifndef BOUNDING_BOX_HELPER
#define BOUNDING_BOX_HELPER

#pragma once

#include "Config.h"
#include "Logger\Logger.h"
#include "td_console.h"

typedef std::pair<COORD, char> cellObj;					// Capable of holding coordinates for the unicode character
														// that is used primarily for console window borders.

class BoundingBoxHelper
{
	public:
		BoundingBoxHelper();
		~BoundingBoxHelper();
		BoundingBoxHelper( TDConsole * );

		std::vector < cellObj > getBoundingBox( int, int, int, int, bool );
		void drawBoundingBox( std::vector < cellObj > );
		void drawBoundingBox( int, int, int, int, bool );

	private:
		// Disable copy constructor and assingment operator.
		BoundingBoxHelper(const BoundingBoxHelper &);
		BoundingBoxHelper & operator=(const BoundingBoxHelper &);

	private:
		TDConsole								*console;	// Instance to the console window's standard output handler.
		std::vector<cellObj>::const_iterator	cit;		// cellObj container iterator.
};

#endif