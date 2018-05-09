//**************************************************************************************************//
// info_pane_view.h                                                                                 //
//                                                                                                  //
// Definition: Implementation file for MainMenuView class.                                          //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //
// 5/24/2012 - This class is now a friend to InputDeviceHandler::processMouseInput().               //
//           - getProperties() removed.                                                             //
//                                                                                                  //
// Notes: This class is responsible for displaying strings which are bounded to arbitrary commands. //
//        This class is also the first one to be drawn after the applications startup               //
//        initialzation. The strings in this class contains properties that allow them to be        //
//        interactive, and will respond to keyboard, mouse click/hover events.                      //
//**************************************************************************************************//

#include "main_menu_view.h"

// Default ctor.
MainMenuView::MainMenuView()
{ }

// Default dtor.
MainMenuView::~MainMenuView()
{ }

// Overloaded ctor.
MainMenuView::MainMenuView( GameState *gs )
{ 
	init ( gs );
}

// Create objects of different types stuffed into a tuple where each will represent an
// interactive string that can change properties depending on the mouse event.
// eg. hover or clicking the object.
// The tuple is created with properties: Object ID, Coordinates, text color attribute, 
// string value | wide char value, is string hoverable or clickable?, currently selected?
void MainMenuView::init( GameState *gs )
{
	this->gs = gs;
	this->console = gs->console;

	wRed = FOREGROUND_RED | FOREGROUND_INTENSITY;
	wWhite = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	wWhiteOnBlack = 0x0F;
	wBlackOnGray = 0x70;

	base = 0;  // Not used, but initialize it anyway.
	baseX = baseY = 0;
	mItemTupleVector.clear();

	width = console->getWidth();
	height = console->getHeight();

	// All strings to be added must have a size equal to 24.
	// They can be padded with space both in front and at the back to
	// reach this maximum limit.
	str = "-= Tower Defense 4310 =-";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) - 12);
	mItemTupleVector.push_back(std::make_tuple(0, cursor, wRed, str, 0, false, false));
	      
	str = "  Start Game ( enter )  ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) - 6);
	mItemTupleVector.push_back(std::make_tuple(1, cursor, wWhite, str, 0, true, true));
	      
	str = "Change Map( c )  -" + gs->map->getMapName();
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) - 4);
	mItemTupleVector.push_back(std::make_tuple(2, cursor, wWhite, str, 0, true, false));

	str = "      Editor ( e )      ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) - 2);
	mItemTupleVector.push_back(std::make_tuple(3, cursor, wWhite, str, 0, true, false));

	str = "       Help ( h )       ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = (height / 2);
	mItemTupleVector.push_back(std::make_tuple(4, cursor, wWhite, str, 0, true, false));

	str = "     About ( a )      ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) + 2);
	mItemTupleVector.push_back(std::make_tuple(5, cursor, wWhite, str, 0, true, false));
	
	str = "      Quit ( esc )      ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) + 4);
	mItemTupleVector.push_back(std::make_tuple(6, cursor, wWhite, str, 0, true, false));

}

// Function: Draw the strings currently in the vector.
void MainMenuView::draw()
{
	for (mItemTupleVectorConstIter = mItemTupleVector.begin(); mItemTupleVectorConstIter != mItemTupleVector.end(); ++mItemTupleVectorConstIter)
	{
		// Sets the string color arbitrarily defined by the input handler.
		SetConsoleTextAttribute(console->getConsoleHandle(), std::get<2>(*mItemTupleVectorConstIter));
		SetConsoleCursorPosition(console->getConsoleHandle(), std::get<1>(*mItemTupleVectorConstIter));
		std::cout << std::get<3>(*mItemTupleVectorConstIter);
	}

	// Reset the text foreground color property to white.
	SetConsoleTextAttribute( console->getConsoleHandle(), wWhite );
}

// Overloaded draw function that accepts a mouse hover event.

void MainMenuView::draw(WORD wParam)
{
	return;
}
