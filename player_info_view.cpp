#include "player_info_view.h"


PlayerInfoView::PlayerInfoView( GameState *gs){
	this->gs = gs;
	bb = new BoundingBoxHelper( gs->console );
}

PlayerInfoView::~PlayerInfoView()
{
	delete bb;
}

void PlayerInfoView::draw() {

	WORD red = FOREGROUND_RED | FOREGROUND_INTENSITY;
	WORD green = FOREGROUND_GREEN;
	WORD white = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;

	int base = gs->console->getBase();
	int width = gs->console->getWidth();

	COORD cursor;

	int baseX = base + 2;
	int baseY = base + 1;

	width = width - 2 * baseX + 1; //calculate width of container
	int height = 5;

	int space = ( width - 4 - 60 ) / 2; // calculate inner width - size of 3 containers / 2
	
	bb->drawBoundingBox( baseX, baseY, width, height, false );

	cursor.X = baseX + 4; cursor.Y = baseY + 2;
	bb->drawBoundingBox( cursor.X -2 , cursor.Y - 1, 20, 3, true );
	SetConsoleTextAttribute(gs->console->getConsoleHandle(), white);
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout<< "Lives:          ";
	cursor.X = cursor.X + 15;
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), red );
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	if( gs->player->get_lives() > 0 && gs->player->get_lives() < 11 ) {
		for( uint32 i = 0; i < gs->player->get_lives(); i ++ ){
			cout<< char(3);
			cursor.X = cursor.X - 1;
			SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
		}
	}
	
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white );

	cursor.X = baseX + 4 + space + 20;
	bb->drawBoundingBox( cursor.X -2 , cursor.Y - 1, 20, 3, true );
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout << "Score: " << std::flush;
	cout << setw(9) << setfill('0') << gs->player->get_score() << std::flush;

	cursor.X = baseX + 4 + 2 * space + 40;
	bb->drawBoundingBox( cursor.X -2 , cursor.Y - 1, 20, 3, true );
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
	cout << "Money: " << std::flush;
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), green );
	cout << setw(9) << setfill( ' ' ) << gs->player->get_money() << std::flush;
	SetConsoleTextAttribute( gs->console->getConsoleHandle(), white );

}

void PlayerInfoView::draw(WORD wParam)
{
	return;
}
