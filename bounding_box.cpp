//**************************************************************************************************//
// bounding_box.cpp                                                                                 //
//                                                                                                  //
// Definition: Implementation file for BoundingBox class.                                            //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 12/21/2012 - Added comments.                                                                     //
//                                                                                                  //
// Notes: This class sets the initialization routines before drawing a bounding box around the      //
//        the console window. It needs to call BoundingBoxHelper's drawBoundingBox member function  //
//        to draw the actual extended ascii characters representing the bounding box.               //
//**************************************************************************************************//
#include "bounding_box.h"

// Default ctor.
BoundingBox::BoundingBox()
{ }

// Default dtor.
BoundingBox::~BoundingBox()
{
	delete bb;
}

// Overloaded ctor.
BoundingBox::BoundingBox( TDConsole *console )
{
	this->console = console;
	bb = new BoundingBoxHelper( console );
	init = true;

	base = console->getBase();
	height = console->getHeight() - 1;
	width = console->getWidth() - 1;

	baseX = baseY = 0;
	cursor.X = cursor.Y = 0;
}

// Function: draw is a proxy to BoundingBoxHelper's drawBoundingBox function
// where the actual drawing of bounding box occurs.
void BoundingBox::draw( void )
{
	if( init ) {
		bb->drawBoundingBox( base, base, width, height, false );
		
		// Compute the top of output console window where the title of game will be rendered.
		COORD dwTopPosition = { ( ( width / 2 ) - 4 ), base };
		SetConsoleCursorPosition ( console->getConsoleHandle(), dwTopPosition );

		// Print title.
		std::cout << " TD4310 ";
		
		//init = false;
	}
}

void BoundingBox::draw(WORD wParam)
{
	return;
}