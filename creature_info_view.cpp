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

#include "creature_info_view.h"

// Default ctor.
CreatureInfoView::CreatureInfoView()
{ }

// Default dtor.
CreatureInfoView::~CreatureInfoView()
{
	delete bb;
}

// Overloaded ctor.
CreatureInfoView::CreatureInfoView(GameState * gs)
{
	init(gs);
}

// Function: initialization routine prior to drawing the CreatureInfoView object.
void CreatureInfoView::init( GameState *gs )
{
	this->gs = gs;
	bb = new BoundingBoxHelper( gs->console );

	base = 0;

	// Calculate base coordinates relative to size of console window.
	baseX	=  ( kCreatureInfoViewWidthBase / 100 ) * gs->console->getWidth ( );
	baseY	=  ( kCreatureInfoViewHeightBase / 100 ) * gs->console->getHeight ( );

	// Calculate dimensions relative to size of console window.
	width	= ( kCreatureInfoViewWidth / 100 ) * gs->console->getWidth ( );
	height	= ( kCreatureInfoViewHeight / 100 ) * gs->console->getHeight ( );
}

// Function: Draws the CreatureInfoView object.
void CreatureInfoView::draw()
{
	bb->drawBoundingBox( baseX, baseY, width, height, false );
	cursor.X = baseX + 3;
	cursor.Y = baseY + 1;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );

	std::cout << "Next Wave:";

	cursor.Y += 2;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	std::cout << ' ' << (int8)157 << std::setw(width - 7) << gs->m_creature_mgr->num_creatures(GOBLIN);      //GOBLIN

	cursor.Y += 1;
	SetConsoleCursorPosition(gs->console->getConsoleHandle(), cursor);
	std::cout << ' ' << (int8)233 << std::setw(width - 7) << gs->m_creature_mgr->num_creatures(ORC);         //ORC

	cursor.Y += 1;
	SetConsoleCursorPosition(gs->console->getConsoleHandle(), cursor);
	std::cout << ' ' << (int8)231 << std::setw(width - 7) << gs->m_creature_mgr->num_creatures(TROLL);         //TROLL

	cursor.Y += 1;
	SetConsoleCursorPosition(gs->console->getConsoleHandle(), cursor);
	std::cout << ' ' << (int8)234 << std::setw(width - 7) << gs->m_creature_mgr->num_creatures(TAUREN);         //TAUREN

	bb->drawBoundingBox(cursor.X-1, cursor.Y-4, width-3, 6, true);

	cursor.Y += 2;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );

	cout << "Level:";

	cursor.Y += 2;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );

	std::cout << setw(width-5) << gs->m_creature_mgr->current_level;
	bb->drawBoundingBox( cursor.X-1, cursor.Y-1, width-3, 3, true );
}

void CreatureInfoView::draw(WORD wParam)
{
	return;
}
