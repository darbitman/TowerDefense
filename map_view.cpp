#include "map_view.h"

MapView::MapView(void)
{
}

MapView::~MapView()
{
	delete bb;
}

MapView::MapView( GameState *gs ) {
	this->gs = gs;
	this->map = gs->map;
	this->console = gs->console;
	bb = new BoundingBoxHelper( console );
}

void MapView::update( GameState *gs ) {
	this->map = gs->map;
	this->console = gs->console;
}

Map * MapView::get_map() {
	return map;
}

void MapView::draw() {

	int baseX = 3;
	int baseY = 7;
	int width = 57;
	int height = 26;

	bb->drawBoundingBox( baseX, baseY, width, height, false );

	int map_width = map->getWidth();
	int map_height = map->getHeight();

	int map_baseX = baseX + width / 2 - map_width / 2;
	int map_baseY = baseY + height / 2 - map_height / 2;

	bb->drawBoundingBox( map_baseX - 1, map_baseY - 1, map_width + 2, map_height + 2, true );

	COORD cursor = { map_baseX, map_baseY };
	SetConsoleCursorPosition ( console->getConsoleHandle(), cursor );

	for( int i = 0; i < map->getHeight(); i++ ) {
		
		SetConsoleCursorPosition ( console->getConsoleHandle(), cursor );
		for( int j = 0; j < map->getWidth(); j ++ ) {
			Coord xy(j, i, 3);
			Model *draw_model = map->get_model( xy );
			SetConsoleTextAttribute(console->getConsoleHandle(), draw_model->get_color());
			if( xy == map->get_start() ) {
				if( gs->gs == map_editor )
					cout << 'S';
				else
					cout << draw_model->get_img();
			}
			else
				cout << draw_model->get_img();
		}
		cursor.Y = cursor.Y + 1;
	}
	
	SetConsoleTextAttribute(console->getConsoleHandle(), 0x0F );
}

void MapView::draw(WORD wParam)
{

	return;
}