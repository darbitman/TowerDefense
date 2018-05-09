// TODO: cleanup

#include "status_view.h"
#include "game_state.h"

StatusView::StatusView( GameState *gs )
{
	init(gs);	
}

StatusView::~StatusView()
{
	delete bb;
}

void StatusView::init(GameState * gs)
{
	this->gs = gs;
	bb = new BoundingBoxHelper(gs->console);

	base = gs->console->getBase();

	baseX =  (kStatusViewWidthBase/100) * gs->console->getWidth();
	baseY =  (kStatusViewHeightBase/100) * gs->console->getHeight();

	width = (kStatusViewWidth/100) * gs->console->getWidth();
	height = (kStatusViewHeight/100) * gs->console->getHeight();

	wRed = FOREGROUND_RED | FOREGROUND_INTENSITY;
	wWhite = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	wWhiteOnBlack = 0x0F;
	wBlackOnGray = 0x70;

	cursor.X = width + 1;
	cursor.Y = baseY + 1;
	wchar_t uparrow (0x2191);
	mItemTupleVector.push_back(std::make_tuple(1, cursor, wWhite, "", uparrow, true, false));

	wchar_t block (0x2592);
	int i;
	for(i = 2; i < height-2; ++i)
	{
		cursor.X = width + 1;
		cursor.Y = baseY + i;
		mItemTupleVector.push_back(std::make_tuple(i, cursor, wWhite, "", block, false, false));
	}

	cursor.X = width + 1;
	cursor.Y = baseY + height - 2;
	wchar_t downarrow (0x2193);
	mItemTupleVector.push_back(std::make_tuple(i+1, cursor, wWhite, "", downarrow, true, false));

	it = messages.begin();
	top = bottom = 0;
	nViewableMessages = height - 2;

	addMessage("Welcome to TD4310!");
}


// TODO: draw and overloaded draw() have duplicate code, need to re-implement. -Ranel

void StatusView::draw()
{
	bb->drawBoundingBox( baseX, baseY, width, height, false );

	//********** Draw the scroll bar ************//

	_setmode(_fileno(stdout), _O_U16TEXT);

	for(mItemTupleVectorConstIter = mItemTupleVector.begin(); mItemTupleVectorConstIter != mItemTupleVector.end(); ++mItemTupleVectorConstIter)
	{
		SetConsoleCursorPosition (gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorConstIter));
		std::wcout << std::get<4>(*mItemTupleVectorConstIter);
	}

	_setmode(_fileno(stdout), _O_TEXT);

	//********** End drawing of scroll bar ************//
	
	if ( messages.size() < ( height - 2 ) )
		it = messages.begin();
	else
		it = messages.end() - ( height - 2 );

	cursor.X = baseX + 2;
	cursor.Y = baseY + 1;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );

	// Draw message strings
	for (; it != messages.end(); ++it)
	{
		// If the length of the string to be displayed exceeds the width of the
		// bounding box inner border then cut the string to appropriate length.
		if((*it).length() >= width-5)
			(*it).resize(width-5);

		std::cout.setf( std::ios::left, std::ios::adjustfield );
		std::cout << std::setw(width - 5) << std::setfill(' ');
		
		if(it == (messages.end() - 2))
			SetConsoleTextAttribute(gs->console->getConsoleHandle(), wWhite);
		else if (it == (messages.end() - 1))
			SetConsoleTextAttribute(gs->console->getConsoleHandle(), wRed);

		std::cout << *it;
		std::cout.setf(std::ios::right, std::ios::adjustfield);
		cursor.Y = cursor.Y + 1;
		SetConsoleCursorPosition (gs->console->getConsoleHandle(), cursor);

	}

	SetConsoleTextAttribute(gs->console->getConsoleHandle(), wWhite);
	
}

void StatusView::draw(WORD wParam)
{
	bb->drawBoundingBox( baseX, baseY, width, height, false );

	// Don't call, will cause StatusView's top-most boundingbox border to flicker.
	//clearView();

	//********** Draw the scroll bar ************//

	_setmode(_fileno(stdout), _O_U16TEXT);

	for(mItemTupleVectorConstIter = mItemTupleVector.begin(); mItemTupleVectorConstIter != mItemTupleVector.end(); ++mItemTupleVectorConstIter)
	{
		SetConsoleCursorPosition (gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorConstIter));
		std::wcout << std::get<4>(*mItemTupleVectorConstIter);
	}

	_setmode(_fileno(stdout), _O_TEXT);

	//********** End drawing scroll bar ************//

	if(messages.size() <= (height - 2))
		return;
	
	cursor.X = baseX + 2;
	cursor.Y = baseY + 1;
	SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );

	// Scroll up
	if(wParam == SB_BUTTON_UP)
	{
		if((bottom - nViewableMessages) <= 0)
			return;

		bottom--;
		top = bottom - nViewableMessages;
		
		for(it = messages.begin() + top; it != messages.begin()+bottom; ++it)
		{
			if( it < messages.begin())
				return;

			if((*it).length() >= width-5)
				(*it).resize(width-5);

			std::cout.setf( std::ios::left, std::ios::adjustfield );
			std::cout << std::setw(width - 5) << std::setfill(' ');

			std::cout << *it;
			std::cout.setf( std::ios::right, std::ios::adjustfield );
			cursor.Y = cursor.Y + 1;
			SetConsoleCursorPosition (gs->console->getConsoleHandle(), cursor);

		}
	}
	else
	{ 
		if(bottom >= messages.size())
			return;

		top = bottom - nViewableMessages + 1;

		for(it = messages.begin() + top; it <= (messages.begin()+bottom); ++it)
		{
			if( it < messages.begin())
				return;

			if((*it).length() >= width-5)
				(*it).resize(width-5);

			std::cout.setf( std::ios::left, std::ios::adjustfield );
			std::cout << setw(width - 5) << setfill(' ');

			if(it == (messages.end() - 2))
				SetConsoleTextAttribute(gs->console->getConsoleHandle(), wWhite);
			else if(it == (messages.end() - 1))
				SetConsoleTextAttribute(gs->console->getConsoleHandle(), wRed);

			std::cout << *it;
			std::cout.setf( std::ios::right, std::ios::adjustfield );
			cursor.Y = cursor.Y + 1;
			SetConsoleCursorPosition (gs->console->getConsoleHandle(), cursor);

		}

		bottom++;

		SetConsoleTextAttribute(gs->console->getConsoleHandle(), wWhite);
	}
}

void StatusView::addMessage( std::string s) {

	if(messages.size() == kMaxItems)
	{
		messages.pop_front();
	}

	messages.push_back( s );

	bottom = messages.size();
	if(bottom > nViewableMessages)
		top = bottom - nViewableMessages;
}

// Clears the StatusView's "window" by filling it with empty characters.
// Note this is just a temporary solution. Need to fill the window width
// depending on the size of StatusView window when changed dynamically.
void StatusView::clearView()
{
	cursor.X = baseX + 1;
	cursor.Y = baseY + 1;
	SetConsoleCursorPosition (gs->console->getConsoleHandle(), cursor);

	for(int i = 0; i < height-2; ++i)
	{
		std::cout << "                                                       ";
		cursor.Y = cursor.Y + 1;
		SetConsoleCursorPosition (gs->console->getConsoleHandle(), cursor);
	}
}
