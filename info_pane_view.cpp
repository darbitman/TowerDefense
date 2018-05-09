//**************************************************************************************************//
// info_pane_view.h                                                                                 //
//                                                                                                  //
// Definition: Header file for InfoPaneView class.                                                  //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //
//                                                                                                  //
// Notes: This class is responsible for drawing the info pane window located above the status view  //
//        window. It is primarily used for outputting strings that represents possible commands for //
//        the game.                                                                                 //
//**************************************************************************************************//

#include "info_pane_view.h"
#include "game_state.h"

// Default ctor.
InfoPaneView::InfoPaneView()
{ }

// Default dtor.
InfoPaneView::~InfoPaneView()
{
	delete bb;
}

// Overloaded ctor.
InfoPaneView::InfoPaneView( GameState *gs )
{
	init(gs);	
}

// Function: initialization routine prior to drawing the CreatureInfoView object.
void InfoPaneView::init(GameState * gs)
{
	this->gs = gs;
	bb = new BoundingBoxHelper(gs->console);

	base = gs->console->getBase();
	
	// Calculate base coordinates relative to size of console window.
	baseX = (kInfoPaneViewWidthBase / 100) * gs->console->getWidth();
	baseY =	(kInfoPaneViewHeightBase / 100) * gs->console->getHeight();

	// Calculate dimensions relative to size of console window.
	width	=  (kInfoPaneViewWidth / 100) * gs->console->getWidth();
	height	=  (kInfoPaneViewHeight / 100) * gs->console->getHeight();

	// Intensified white foreground color for the character will be the default text attribute.
	wRed = FOREGROUND_RED | FOREGROUND_INTENSITY;
	wWhite = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	wWhiteOnBlack = 0x0F;
	wBlackOnGray = 0x70;

	//**************** Scrollbar init ****************//

	// Create properties for the up arrow.
	cursor.X = width + 1;
	cursor.Y = baseY + 1;
	wchar_t uparrow (0x2191);
	mItemTupleVector.push_back(std::make_tuple(1, cursor, wWhite, "", uparrow, true, false));

	// Create properties for all blocks (middle of scrollbar).
	wchar_t block (0x2592);
	int i;
	for(i = 2; i < height-2; ++i)
	{
		cursor.X = width + 1;
		cursor.Y = baseY + i;
		mItemTupleVector.push_back(std::make_tuple(i, cursor, wWhite, "", block, false, false));
	}

	// Create properties for the down arrow.
	cursor.X = width + 1;
	cursor.Y = baseY + height - 2;
	wchar_t downarrow (0x2193);
	mItemTupleVector.push_back(std::make_tuple(++i, cursor, wWhite, "", downarrow, true, false));

	it = messages.begin();
	top = bottom = 0;
	nViewableMessages = height - 2;

	//**************** End scrollbar init ****************//


	//**************** Edit control commands init ****************//

	i = 0;

	str = "Edit Mode:               \n";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 1;
	mEditControlCommands.push_back(std::make_tuple(i, cursor, wRed, str, 0, false, false));

	str = "Enter Build Mode ( B )   ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 2;
	mEditControlCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	str = "Start the Next Wave ( S )";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 3;
	mEditControlCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, true));

	str = "Quit ( ESC )             ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 4;
	mEditControlCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	//**************** End Edit control commands init ****************//

	//**************** Build commands init ****************//

	i = 0;

	str = "Build Mode:                     ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 1;
	mBuildCommands.push_back(std::make_tuple(i, cursor, wRed, str, 0, false, false));

	str = "Change Tower Type ( TAB )       ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 2;
	mBuildCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, true));

	str = "Place Tower ( ENTER )           ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 3;
	mBuildCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	str = "Sell Tower ( DEL )              ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 4;
	mBuildCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	str = "Go Back to Previous Menu ( ESC )";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 5;
	mBuildCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	str = "Use Arrow Keys to Move Tower    ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 6;
	mBuildCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, false, false));

	//**************** End Build commands init ****************//

	//**************** Map Editor commands init ****************//

	i = 0;

	str = "Map Editor Mode:                ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 1;
	mMapEditorCommands.push_back(std::make_tuple(i, cursor, wRed, str, 0, false, false));

	str = "Cycle Between Tiles ( TAB )     ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 2;
	mMapEditorCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, true));

	str = "Change Tile ( ENTER )           ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 3;
	mMapEditorCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	str = "Save ( S )                      ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 4;
	mMapEditorCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	str = "Load ( L )                      ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 5;
	mMapEditorCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	str = "Previous Menu ( ESC )           ";
	cursor.X = baseX + 2;
	cursor.Y = baseY + 6;
	mMapEditorCommands.push_back(std::make_tuple(++i, cursor, wWhite, str, 0, true, false));

	//**************** End Map Editor commands init ****************//

	initialized = execonce = false;
}

// Function: If string is appended to the deque container, it needs to be drawn.
// This function is responsible for drawing the updated deque container.
void InfoPaneView::draw()
{
	bb->drawBoundingBox( baseX, baseY, width, height, false );

	// Clear this view's "window" area first.
	//clearView();

	//********** Begin drawing the scroll bar ************//
	
	_setmode(_fileno(stdout), _O_U16TEXT);

	for(mItemTupleVectorConstIter = mItemTupleVector.begin(); mItemTupleVectorConstIter != mItemTupleVector.end(); ++mItemTupleVectorConstIter)
	{
		SetConsoleCursorPosition (gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorConstIter));
		std::wcout << std::get<4>(*mItemTupleVectorConstIter);
	}

	_setmode(_fileno(stdout), _O_TEXT);
	
	//********** End drawing of scroll bar ************//

	if(gs->gs == run_mode)
	{
			execonce = true;
			// This formula allows the drawing of strings n messages at a time depending on the current size of the info pane window. 
			if ( messages.size() < ( height - 2 ) )
				it = messages.begin();
			else
				it = messages.end() - ( height - 2 );

			// Set the upper-left location, within the info pane view, where the first string will be drawn.
			cursor.X = baseX + 2;
			cursor.Y = baseY + 1;
			SetConsoleCursorPosition ( gs->console->getConsoleHandle(), cursor );
			SetConsoleTextAttribute(gs->console->getConsoleHandle(), wWhite);

			// Draw the rest of the message strings.
			for (; it != messages.end(); ++it)
			{
				// If the length of the string to be displayed exceeds the width of the
				// bounding box inner border then cut the string to appropriate length.
				if((*it).length() >= width-5)
					(*it).resize(width-5);

				std::cout.setf( ios::left, ios::adjustfield );
				std::cout << setw(width - 5) << std::setfill(' ');

				std::cout << *it;
				std::cout.setf(ios::right, ios::adjustfield);
				cursor.Y = cursor.Y + 1;

				// Adjust the position of where to draw the next string accordingly.
				SetConsoleCursorPosition (gs->console->getConsoleHandle(), cursor);
			}
		return;
	}
	
	if(gs->gs == edit_mode)
	{
		mItemTupleVectorIter = mEditControlCommands.begin();
		mItemTupleVectorConstIter = mEditControlCommands.end();
	}
	else if(gs->gs == build_mode)
	{
		mItemTupleVectorIter = mBuildCommands.begin();
		mItemTupleVectorConstIter = mBuildCommands.end();
	}
	else if(gs->gs == map_editor)
	{
		mItemTupleVectorIter = mMapEditorCommands.begin();
		mItemTupleVectorConstIter = mMapEditorCommands.end();
	}

	// Begin drawing strings...
	for (; mItemTupleVectorIter != mItemTupleVectorConstIter; ++mItemTupleVectorIter)
	{
		// Sets the string color arbitrarily defined by the input handler.
		SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(*mItemTupleVectorIter));
		SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorIter));

		std::cout << std::get<3>(*mItemTupleVectorIter);
	}

	// Reset the text foreground color property to white.
	SetConsoleTextAttribute(gs->console->getConsoleHandle(), wWhite);
}

// Function: Primarily used for mouse hover event.
void InfoPaneView::draw(WORD wParam)
{
	clearView();
	draw();
	return;
}

// Function: Appends a new string to the end of the deque.
void InfoPaneView::addMessage(std::string s)
{
	// If the maximu allowed number of string to store has been reached,
	// remove the front.
	if(messages.size() == kMaxItems)
	{
		messages.pop_front();
	}

	// ... then push back the current string.
	messages.push_back( s );

	// When a new string is appended to the end of the queue,
	// the top and bottom location needs to be updated so they
	// can be drawn properly.
	bottom = messages.size();
	if(bottom > nViewableMessages)
		top = bottom - nViewableMessages;
}

// Function: Clears the vector by popping all the message strings.
void InfoPaneView::clear_messages()
{
	messages.clear();
}

// Clears the InfoPaneView's "window" by filling it with empty characters.
// Note this is just a temporary solution. Need to fill the window width
// depending on the size of InfoPaneView window when changed dynamically.
void InfoPaneView::clearView()
{
	cursor.X = baseX + 1;
	cursor.Y = baseY + 1;
	SetConsoleCursorPosition (gs->console->getConsoleHandle(), cursor);

	for(int i = 0; i < height-2; ++i)
	{
		std::cout << "                                                   ";
		cursor.Y = cursor.Y + 1;
		SetConsoleCursorPosition (gs->console->getConsoleHandle(), cursor);
	}
}