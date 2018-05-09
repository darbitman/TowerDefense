//**************************************************************************************************//
// bounding_box.h                                                                                   //
//                                                                                                  //
// Definition: Header file for BoundingBox class                                                    //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 12/21/2012 - Added comments.                                                                     //
//                                                                                                  //
// Notes: This class sets the initialization routines before drawing a bounding box around the      //
//        the console window. It needs to call BoundingBoxHelper's drawBoundingBox member function  //
//        to draw the actual extended ascii characters representing the bounding box.               //
//**************************************************************************************************//

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#pragma once

#include "view.h"
#include "Config.h"
#include "Logger\Logger.h"
#include "td_console.h"
#include "bounding_box_helper.h"

class BoundingBox :
	public View
{
	public:
		BoundingBox();
		~BoundingBox();
		BoundingBox( TDConsole * );

		virtual void draw( void );
		virtual void draw(WORD);

	private:
		// Disable default copy constructor and assignment operator.
		BoundingBox(const BoundingBox &);
		BoundingBox & operator=(const BoundingBox &);

	private:
		TDConsole								*console;		// Instance to the console window's standard output handler.
		BoundingBoxHelper						*bb;			// Instance to BoundingBoxHelper class.
		bool									init;
};

#endif
