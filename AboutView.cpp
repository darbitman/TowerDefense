#include "AboutView.h"
#include "game_state.h"

AboutView::AboutView()
{ }

AboutView::AboutView(GameState * gs)
{
	init(gs);
}

AboutView::~AboutView()
{
	delete bb;
}

void AboutView::init(GameState * gs)
{
	this->gs = gs;
	bb = new BoundingBoxHelper(gs->console);

	wRed = FOREGROUND_RED | FOREGROUND_INTENSITY;
	wWhite = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	wWhiteOnBlack = 0x0F;
	wBlackOnGray = 0x70;

	base = gs->console->getBase();
	baseX = baseY = 0;
	mItemTupleVector.clear();

	width = gs->console->getWidth();
	height = gs->console->getHeight();

	str = "  -= Credits =-  ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) - 12);
	mItemTupleVector.push_back(std::make_tuple(0, cursor, wRed, str, 0, false, false));

	str = "   Dmitriy A.    ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) - 4);
	mItemTupleVector.push_back(std::make_tuple(1, cursor, wWhite, str, 0, false, false));

	str = "    Ranel D.     ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) - 2);
	mItemTupleVector.push_back(std::make_tuple(2, cursor, wWhite, str, 0, false, false));

	str = "     John N.     ";
	cursor.X = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2));
	mItemTupleVector.push_back(std::make_tuple(3, cursor, wWhite, str, 0, false, false));

	str = "    Julie S.     ";
	cursor.Y = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) + 2);
	mItemTupleVector.push_back(std::make_tuple(4, cursor, wWhite, str, 0, false, false));

	str = "    Joshua S.    ";
	cursor.Y = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) + 4);
	mItemTupleVector.push_back(std::make_tuple(5, cursor, wWhite, str, 0, false, false));

	str = "    Continue     ";
	cursor.Y = ((width / 2) - str.length() / 2);
	cursor.Y = ((height / 2) + 14);
	mItemTupleVector.push_back(std::make_tuple(6, cursor, wWhite, str, 0, true, true));

}

void AboutView::draw()
{
	for (mItemTupleVectorConstIter = mItemTupleVector.begin(); mItemTupleVectorConstIter != mItemTupleVector.end(); ++mItemTupleVectorConstIter)
	{
		// Sets the string color arbitrarily defined by the input handler.
		SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(*mItemTupleVectorConstIter));
		SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorConstIter));
		std::cout << std::get<3>(*mItemTupleVectorConstIter);
	}

	// Reset the text foreground color property to white.
	SetConsoleTextAttribute(gs->console->getConsoleHandle(), wWhite);
}

void AboutView::draw(WORD wParam)
{
	return;
}