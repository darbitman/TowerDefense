#include "tile_info_view.h"


TileInfoView::TileInfoView( GameState *gs ){
	this->gs = gs;
	bb = new BoundingBoxHelper( gs->console );
	this->selection = 0;
}

TileInfoView::~TileInfoView()
{
	delete bb;
}

void TileInfoView::selectionInc() { 
	this->selection = ( selection + 1 ) % 5;
}

int TileInfoView::getSelection() { return selection; }

void TileInfoView::draw(){

	WORD black_on_black= 0x00;
	WORD white_on_black = 0x0F;
	WORD grass = 0x20;
	WORD path = 0x70;
	WORD rocks = 0x28;

	int base = gs->console->getBase();
	int width = gs->console->getWidth();

	COORD cursor;

	int baseX = base + 60;
	int baseY = base + 6;

	width = width - baseX - 2; //calculate width of container
	int height = 26;

	bb->drawBoundingBox( baseX, baseY, width, height, false );
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	
	cursor.X = baseX + 3;
	cursor.Y = baseY + 2;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout << "Path     ";
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), path );
	cout << ' ';
	if( selection == 0 )
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	else
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), black_on_black );
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );

	cursor.Y = baseY + 5;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout << "Grass    ";
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), grass );
	cout << ' ';
	if( selection == 1 )
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	else
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), black_on_black );
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );

	cursor.Y = baseY + 8;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout << "Rocks    ";
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), rocks );
	cout << (char) 178;
	if( selection == 2 )
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	else
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), black_on_black );
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );

	cursor.Y = baseY + 11;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout <<"Base     ";
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), path );
	cout << (char) 127;
	if( selection == 3 )
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	else
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), black_on_black );
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );

	cursor.Y = baseY + 14;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout <<"Start    ";
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), path );
	cout << 'S';
	if( selection == 4 )
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );
	else
		SetConsoleTextAttribute( gs->console->getConsoleHandle(), black_on_black );
	bb->drawBoundingBox( cursor.X - 1 , cursor.Y - 1, width - 4, 3, true );
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white_on_black );

}

void TileInfoView::draw(WORD wParam)
{
	return;
}