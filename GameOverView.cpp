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

#include "GameOverView.h"
#include "game_state.h"
#include <sstream>

// Default ctor.
GameOverView::GameOverView()
{ }

// Default dtor.
GameOverView::~GameOverView ( )
{
	delete bb;
}

// Overloaded ctor.
GameOverView::GameOverView(GameState * gs)
{
	init(gs);
}

// Initialize all console related properties here.
void GameOverView::init(GameState * gs)
{
	this->gs = gs;
	bb = new BoundingBoxHelper(gs->console);

	wRed = FOREGROUND_RED | FOREGROUND_INTENSITY;
	wWhite = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	wWhiteOnBlack = 0x0F;
	wBlackOnGray = 0x70;

	base = gs->console->getBase();

	width = 50;
	height = 25;
	
	baseX = gs->console->getWidth() / 2 - width / 2;
	baseY = gs->console->getHeight() / 2 - height / 2;

	str = "Game Over!";
	cursor.X = (baseX + (width / 2) - str.size() / 2);
	cursor.Y = baseY + 5;
	mItemTupleVector.push_back(std::make_tuple(0, cursor, wWhite, str, 0, false, false));

	str = "Score: ";
	cursor.X = baseX + 10;
	cursor.Y = baseY + 11;
	mItemTupleVector.push_back(std::make_tuple(1, cursor, wWhite, str, 0, false, false));

	std::stringstream score;
	score << gs->player->get_score();
	cursor.X = baseX + 25;
	cursor.Y = baseY + 11;
	str = score.str();
	mItemTupleVector.push_back(std::make_tuple(2, cursor, wRed, str, 0, false, false));

	str = "Level: ";
	cursor.X = baseX + 10;
	cursor.Y = baseY + 14;
	mItemTupleVector.push_back(std::make_tuple(3, cursor, wWhite, str, 0, false, false));

	std::stringstream level;
	level << gs->creatureMgr()->get_current_level() - 1;
	cursor.X = baseX + 25;
	cursor.Y = baseY + 14;
	str = level.str();
	mItemTupleVector.push_back(std::make_tuple(4, cursor, wRed, str, 0, false, false));

	str = "        Continue        ";
	cursor.X = (baseX + (width / 2) - str.size() / 2);
	cursor.Y = baseY + 20;
	mItemTupleVector.push_back(std::make_tuple(5, cursor, wWhite, str, 0, true, true));
}

// Override View's draw() virtual member function.
void GameOverView::draw()
{
	CHAR_INFO mRect[25][50];
	COORD base; base.X = 0; base.Y = 0;
	COORD dwSize; dwSize.X = width; dwSize.Y = height;
	SMALL_RECT srRect;
	srRect.Top = baseY; srRect.Left = baseX;
	srRect.Bottom = baseY+height; srRect.Right = baseX+width;

	// Fill the rectangular box with empty characters.
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			mRect[i][j].Char.UnicodeChar = ' ';
			mRect[i][j].Attributes = 0;
		}
	}

	// Draw an arbitrary sized rectangular block inside the console output.
	if(!WriteConsoleOutput(gs->console->getConsoleHandle(), *mRect, dwSize, base, &srRect))
	{
		CLogger::instance().log("Unable to read input records in InputDeviceHandler::poll()",
										CLogger::kLogLevelDebug);
	}

	// Draw an arbitrary bounding box around this rectangular block.
	bb->drawBoundingBox(baseX, baseY, width, height, false);
	
	if(wStatus)
	{
		for (mItemTupleVectorConstIter = mItemTupleVector.begin(); mItemTupleVectorConstIter != mItemTupleVector.end(); ++mItemTupleVectorConstIter)
		{
			// Sets the string color arbitrarily defined by the input handler.
			SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(*mItemTupleVectorConstIter));
			SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorConstIter));
			std::cout << std::get<3>(*mItemTupleVectorConstIter);
		}
	}
}

void GameOverView::draw(WORD wParam)
{
	return;
}

void GameOverView::setStatus(WORD wParam)
{
	wStatus = wParam;
}
