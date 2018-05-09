#pragma once

//#include <process.h>
#include "Config.h"
#include "Logger\Logger.h"

class TDConsole
{
	public:
		TDConsole(void);

		HANDLE getConsoleHandle( void );
		HANDLE getConsoleInputHandle ( void );
		SHORT getBase( void );
		SHORT getHeight( void );
		SHORT getWidth( void );
		void clearConsole();

		~TDConsole(void);

	private:	
		HANDLE						hStdOut,						// Input and output handles
									hStdIn;							// for console.
		HWND						hwndConsole;					// Handle to console window.
		DWORD						fdwSaveOldMode;					// Holds the previous mode of console.
		SHORT						shBase,							// Bounding height for the window border.
									shRelativeHeight,				// Minus 2 units from the absolute height of console window.
									shRelativeWidth;				// Minus 2 units from the absolute width of console window.
		COORD						dwConsoleScreenBufferSize;		// Holds the size of the std i/o console's buffer size.
		SMALL_RECT					dwConsoleWindowSize;			// The size of the console window in console character unit.
		RECT						rectConsoleWindowSizePix;		// Holds the size of the console window in pixel unit.
		CONSOLE_CURSOR_INFO			dwPrevConsoleCursorInfo,		// Holds the previous cursor state.
									dwConsoleCursorInfo;			// Holds the new cursor state.
		CONSOLE_SCREEN_BUFFER_INFO	dwConsoleScreenBufferInfo;		// Holds the previous console screen buffer info.
};

