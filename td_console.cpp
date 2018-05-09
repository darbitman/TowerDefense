#include "td_console.h"


TDConsole::TDConsole(void)
{
	// Get standard input/output handles.
	hStdOut	=	GetStdHandle ( STD_OUTPUT_HANDLE );
	hStdIn	=	GetStdHandle ( STD_INPUT_HANDLE );

	// Check validity of standard input/output handles.
	if ( hStdOut == INVALID_HANDLE_VALUE || hStdIn == INVALID_HANDLE_VALUE )
	{
		CLogger::instance().log ( "Invalid Input/Output Handle in TDConsole::TDConsole().",
									CLogger::kLogLevelError );
		std::exit ( EXIT_FAILURE );
	}

	// Attempt to save current input console mode.
	if ( ! GetConsoleMode ( hStdIn, &fdwSaveOldMode ) )
	{
		CLogger::instance().log ( "Unable to save current input console mode in TDConsole::TDConsole().",
									CLogger::kLogLevelDebug );
	}

	// Attempt to save ouput console properties.
	if ( ! GetConsoleScreenBufferInfo ( hStdOut, &dwConsoleScreenBufferInfo ) )
	{
		CLogger::instance().log ( "Unable to save current console output buffer info in TDConsole::TDConsole().",
									CLogger::kLogLevelDebug );
	}

	// Attempt to save ouput console cursor properties.
	if ( ! GetConsoleCursorInfo ( hStdOut, &dwPrevConsoleCursorInfo ) )
	{
		CLogger::instance().log ( "Unable to save output console's cursor properties in TDConsole::TDConsole().",
									CLogger::kLogLevelDebug );
	}

	// Set new size for output console screen buffer.
	dwConsoleScreenBufferSize.X = CONSOLE_WINDOW_WIDTH;
	dwConsoleScreenBufferSize.Y = CONSOLE_WINDOW_HEIGHT;

	// Attempt to set the output console's screen buffer size
	if ( ! SetConsoleScreenBufferSize ( hStdOut, dwConsoleScreenBufferSize ) )
	{
		CLogger::instance().log ( "Unable to set new screen buffer size in TDConsole::TDConsole().",
				CLogger::kLogLevelDebug );
	}
	
	// Set new size for output console window. This will make the console window's vertical scrollbar disappear.
	dwConsoleWindowSize.Top		= 0;
	dwConsoleWindowSize.Left	= 0;
	dwConsoleWindowSize.Right	= CONSOLE_WINDOW_WIDTH - 1;
	dwConsoleWindowSize.Bottom	= CONSOLE_WINDOW_HEIGHT - 1;

	// Attempt to set the new size for output console window.
	if ( ! SetConsoleWindowInfo( hStdOut, TRUE, &dwConsoleWindowSize ) )
	{
		CLogger::instance().log ( "Unable to set new console window size in TDConsole::TDConsole().",
									CLogger::kLogLevelDebug );
	}

	// Set new cursor properties.
	dwConsoleCursorInfo.bVisible	= false;
	dwConsoleCursorInfo.dwSize		= 1;

	// Attempt to set new properties for the cursor.
	if ( ! SetConsoleCursorInfo ( hStdOut, &dwConsoleCursorInfo ) )
	{
		CLogger::instance().log ( "Unable to set new console cursor info in TDConsole::TDConsole().",
									CLogger::kLogLevelDebug );
	}
	
	// Set console window's relative width and height.
	shRelativeWidth		= CONSOLE_WINDOW_WIDTH - 1;
	shRelativeHeight	= CONSOLE_WINDOW_HEIGHT - 1;

	// Set upper-left X coordinate value.
	shBase = 1;

	// Attempt to set the title for the output console window.
	if ( ! SetConsoleTitle ( L"Tower Defense" ) )
	{
		CLogger::instance().log ( "Unable to set the title for the console window in TDConsole::TDConsole().",
									CLogger::kLogLevelDebug );
	}


	//** Center the console window. **//

	hwndConsole = GetConsoleWindow ( );

	if ( hwndConsole != NULL )
	{
		// Get console window position (upper-left, lower-right) in pixels.
		GetWindowRect ( hwndConsole, &rectConsoleWindowSizePix );
		
		// Get console window dimensions in pixels.
		int nWidth	= rectConsoleWindowSizePix.right - rectConsoleWindowSizePix.left;
		int nHeight	= rectConsoleWindowSizePix.bottom - rectConsoleWindowSizePix.top;

		// Get screen dimensions.
		int nScreenWidth  = GetSystemMetrics ( SM_CXSCREEN );
		int nScreenHeight = GetSystemMetrics ( SM_CYSCREEN );

		// Calculate position of upper-left position of console window.
		int nX = ( nScreenWidth / 2 ) - ( nWidth / 2 );
		int nY = ( nScreenHeight / 2 ) - ( nHeight / 2 );

		// Attempt to set the console window position to center of desktop.
		if ( ! SetWindowPos ( hwndConsole, NULL, nX, nY, 0, 0, SWP_NOSIZE ) )
		{
			CLogger::instance().log ( "Unable to center the console window in TDConsole::TDConsole().",
									CLogger::kLogLevelDebug );
		}
	}

}

HANDLE TDConsole::getConsoleHandle( void ){ return hStdOut; }
HANDLE TDConsole::getConsoleInputHandle( void ) { return hStdIn; }
SHORT TDConsole::getBase( void ) { return shBase; }
SHORT TDConsole::getHeight( void ) { return shRelativeHeight; }
SHORT TDConsole::getWidth( void ) { return shRelativeWidth; }

TDConsole::~TDConsole(void)
{
	// If call to destructor fails, memory leak will occur.
	SetConsoleCursorInfo		( hStdOut, &dwPrevConsoleCursorInfo );
	SetConsoleTextAttribute		( hStdOut, dwConsoleScreenBufferInfo.wAttributes );
	SetConsoleScreenBufferSize	( hStdOut, dwConsoleScreenBufferInfo.dwSize );
	SetConsoleWindowInfo		( hStdOut, TRUE, &dwConsoleScreenBufferInfo.srWindow );
	SetConsoleMode				( hStdIn, fdwSaveOldMode);
}

void TDConsole::clearConsole()
{
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; 
	DWORD dwConSize;

	if( !GetConsoleScreenBufferInfo( hStdOut, &csbi ))
	{
		return;
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.

	if( !FillConsoleOutputCharacter( hStdOut,        // Handle to console screen buffer 
									(TCHAR) ' ',     // Character to write to the buffer
									dwConSize,       // Number of cells to write 
									coordScreen,     // Coordinates of first cell 
									&cCharsWritten ))// Receive number of characters written
	{
		return;
	}

	// Get the current text attribute.

	if( !GetConsoleScreenBufferInfo( hStdOut, &csbi ))
	{
	return;
	}

	// Set the buffer's attributes accordingly.

	if( !FillConsoleOutputAttribute( hStdOut,         // Handle to console screen buffer 
								csbi.wAttributes, // Character attributes to use
								dwConSize,        // Number of cells to set attribute 
								coordScreen,      // Coordinates of first cell 
								&cCharsWritten )) // Receive number of characters written
	{
	return;
	}

	// Put the cursor at its home coordinates.

	SetConsoleCursorPosition( hStdOut, coordScreen );
}