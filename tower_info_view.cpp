#include "tower_info_view.h"

TowerInfoView::TowerInfoView( GameState *gs ){
	this->gs = gs;
	bb = new BoundingBoxHelper( gs->console );
	this->selection = 0;
}

TowerInfoView::~TowerInfoView()
{
	delete bb;
}

void TowerInfoView::selectionInc() { 
	this->selection = ( selection + 1 ) % 3;
}
int TowerInfoView::getSelection() { return selection; }

void TowerInfoView::draw(){

	WORD black_on_grey= 0x70;
	WORD white_on_black = 0x0F;

	int base = gs->console->getBase();
	int width = gs->console->getWidth();

	COORD cursor;

	int baseX = base + 60;
	int baseY = base + 6;

	width = width - baseX - 2; //calculate width of container
	int height = 26;

	bb->drawBoundingBox( baseX, baseY, width, height, false );

	if( selection == 0 )
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), black_on_grey );
	cursor.X = baseX + 3;
	cursor.Y = baseY + 2;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout << char(227) << ':';
	cout << setw( width - 8) << setfill(' ') << "$50";
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );

	if( selection == 1 )
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), black_on_grey );
	cursor.Y = baseY + 5;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout << '+' << ':';
	cout << setw( width - 8) << setfill(' ') << "$75";
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );

	if( selection == 2 )
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), black_on_grey );
	cursor.Y = baseY + 8;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout << char(248) << ':';
	cout << setw( width - 8) << setfill(' ') << "$100";
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );

	cursor.Y = baseY + 11;
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );
}

void TowerInfoView::draw(WORD wParam)
{
	return;
}
