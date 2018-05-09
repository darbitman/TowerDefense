//**************************************************************************************************//
// filename_view.h                                                                                  //
//                                                                                                  //
// Definition: Header file for FilenameView class                                                   //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //
//                                                                                                  //
// Notes: This class is responsible for drawing the file name input window that prompts for file    //
//        name. Located at the center of the console window.                                        //
//**************************************************************************************************//

#include "filename_view.h"

// Default ctor.
FilenameView::FilenameView()
{ }

// Default dtor.
FilenameView::~FilenameView()
{
	delete bb;

	if(!WriteConsoleOutput(gs->console->getConsoleHandle(), chiBuffer, coordBufSize, coordBufCoord, &srctReadRect))
	{
		CLogger::instance().log("Unable to write output buffer to console in InputDeviceHandler::poll()",
										CLogger::kLogLevelDebug);
	}
}

// Overloaded ctor.
FilenameView::FilenameView( GameState *gs, WORD wParam )
{
	wState = wParam;
	init(gs);
}

// Function: Initialization routine prior to drawing the FilenameView object.
void FilenameView::init(GameState * gs)
{
	this->gs = gs;
	bb = new BoundingBoxHelper( gs->console );

	width = 45;
	height = 27;

	base = gs->console->getBase();

	baseX = gs->console->getWidth() / 2 - width / 2;
	baseY = gs->console->getHeight() / 2 - height / 2 - 3;

	//************** Console box properties we want to save before we overwrite *************//
	coordBufSize.Y = height;
	coordBufSize.X = width;

	coordBufCoord.Y = 0;
	coordBufCoord.X = 0;

	srctReadRect.Top = baseY;
	srctReadRect.Left = baseX;
	srctReadRect.Bottom = baseY + height;
	srctReadRect.Right = baseX + width;

	if(!ReadConsoleOutput(gs->console->getConsoleHandle(), chiBuffer, coordBufSize, coordBufCoord, &srctReadRect))
	{
		return;
	}

	//************** End *************//

	//************** Console box properties we will use to overwrite on top of previous box  *************//

	CHAR_INFO mRect[27][45];
	COORD cBase; cBase.X = 0; cBase.Y = 0;
	COORD dwSize; dwSize.X = width; dwSize.Y = height;
	SMALL_RECT srRect;
	srRect.Top = baseY; srRect.Left = baseX;
	srRect.Bottom = baseY + height; srRect.Right = baseX + width;

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
	if(!WriteConsoleOutput(gs->console->getConsoleHandle(), *mRect, dwSize, cBase, &srRect))
	{
		CLogger::instance().log("Unable to read input records in InputDeviceHandler::poll()",
										CLogger::kLogLevelDebug);
	}

	//************** End drawing blank box of 27 x 45 on top of Map Edit view *************//

	// Bounding box 1 (Outer border)
	bd1.baseX = baseX;
	bd1.baseY = baseY;
	bd1.width = width;
	bd1.height = height;
	bd1.single = false;

	// Draw an arbitrary bounding box around this rectangular block.
	bb->drawBoundingBox(bd1.baseX, bd1.baseY, bd1.width, bd1.height, bd1.single);


	// Bounding box 2 (File name output and dimensions)
	bd2.baseX = baseX + 3;
	bd2.baseY = baseY + 4;
	bd2.width = width - 6;
	bd2.height = height - 12;
	bd2.single = true;

	// Draw another bounding box inside the outer bounding box.
	bb->drawBoundingBox(bd2.baseX, bd2.baseY, bd2.width, bd2.height, bd2.single);

	// Bounding box 3 (File name input)
	bd3.baseX = baseX + 3;
	bd3.baseY = baseY + 19;
	bd3.width = width - 14;
	bd3.height = 3;

	// Draw a bounding box for entering a file name.
	bb->drawBoundingBox(bd3.baseX, bd3.baseY, bd3.width, bd3.height, bd3.single);

	// Bounding box 4 (File name extension)
	bd4.baseX = baseX + 34;
	bd4.baseY = baseY + 19;
	bd4.width = width - 37;
	bd4.height = 3;
	bd4.single = true;

	// Draw a bounding box for static view of the file name extension.
	bb->drawBoundingBox(bd4.baseX, bd4.baseY, bd4.width, bd4.height, bd4.single);

	//********** Auxillary strings properties ***********//

	wRed = FOREGROUND_RED | FOREGROUND_INTENSITY;
	wWhite = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	wWhiteOnBlack = 0x0F;
	wBlackOnGray = 0x70;

	if(wState == LOAD_MAP)
	{
		str = "Load Map";
		cursor.X = gs->console->getWidth() / 2 - str.length() / 2;
		cursor.Y = baseY + 2;
		mItemTupleVector.push_back(std::make_tuple(0, cursor, wRed, str, 0, false, false));
	}
	else
	{
		str = "Save Map";
		cursor.X = gs->console->getWidth() / 2 - str.length() / 2;
		cursor.Y = baseY + 2;
		mItemTupleVector.push_back(std::make_tuple(+1, cursor, wRed, str, 0, false, false));
	}

	str = "Name";
	cursor.X = bd2.baseX + ((bd2.width / 2) / 2) - (str.length() / 2);
	cursor.Y = bd2.baseY + 2;
	mItemTupleVector.push_back(std::make_tuple(2, cursor, wRed, str, 0, false, false));

	str = "Dimensions";
	cursor.X = bd2.baseX + (bd2.width - (bd2.width / 2) / 2) - (str.length() / 2);
	cursor.Y = bd2.baseY + 2;
	mItemTupleVector.push_back(std::make_tuple(3, cursor, wRed, str, 0, false, false));

	str = ".tdm";
	cursor.X = bd4.baseX + 2;
	cursor.Y = bd4.baseY + 1;
	mItemTupleVector.push_back(std::make_tuple(4, cursor, wRed, str, 0, false, false));

	if(wState == LOAD_MAP)
	{
		str = "     Load     ";
		cursor.X = bd1.baseX + ((bd1.width / 2) / 2) - (str.length() / 2) + 2;
		cursor.Y = bd1.baseY + bd1.height - 3;
		mItemTupleVector.push_back(std::make_tuple(99, cursor, wWhite, str, 0, true, true));
	}
	else
	{
		str = "     Save     ";
		cursor.X = bd1.baseX + ((bd1.width / 2) / 2) - (str.length() / 2) + 2;
		cursor.Y = bd1.baseY + bd1.height - 3;
		mItemTupleVector.push_back(std::make_tuple(100, cursor, wWhite, str, 0, true, true));
	}

	str = "    Cancel    ";
	cursor.X = bd1.baseX + (bd1.width - (bd1.width / 2) / 2) - (str.length() / 2) - 2;
	cursor.Y = bd1.baseY + bd1.height - 3;
	mItemTupleVector.push_back(std::make_tuple(101, cursor, wWhite, str, 0, true, false));

	cursor.X = bd3.baseX + 1;
	cursor.Y = bd3.baseY + 1;
	mItemTupleVector.push_back(std::make_tuple(8, cursor, wWhite, "_", 0, false, false));	

	//************ Begin scrollbar routine ************//

	cursor.X = bd2.baseX + bd2.width - 2;
	cursor.Y = bd2.baseY + 4;
	wchar_t uparrow (0x2191);
	mItemTupleVector.push_back(std::make_tuple(9, cursor, wWhite, "", uparrow, true, false));

	int i = 10;
	wchar_t block (0x2592);
	for(int j = 5; j < bd2.height - 2; ++j)
	{
		cursor.X = bd2.baseX + bd2.width - 2;
		cursor.Y = bd2.baseY + j;
		mItemTupleVector.push_back(std::make_tuple(i++, cursor, wWhite, "", block, false, false));
	}

	cursor.X = bd2.baseX + bd2.width - 2;
	cursor.Y = bd2.baseY + bd2.height - 2;
	wchar_t downarrow (0x2193);
	mItemTupleVector.push_back(std::make_tuple(++i, cursor, wWhite, "", downarrow, true, false));

	// Draw the scrollbar and all other auxillary strings.
	for (mItemTupleVectorIter = mItemTupleVector.begin(); mItemTupleVectorIter != mItemTupleVector.end(); ++mItemTupleVectorIter)
	{
		// Sets the string color arbitrarily defined by the input handler.
		SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(*mItemTupleVectorIter));
		SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorIter));
		if(std::get<4>(*mItemTupleVectorIter) != 0)
		{
			_setmode(_fileno(stdout), _O_U16TEXT);
			std::wcout << std::get<4>(*mItemTupleVectorIter);
			_setmode(_fileno(stdout), _O_TEXT);
		}
		else
			std::cout << std::get<3>(*mItemTupleVectorIter);
	}

	//************ End scrollbar routine ************//


	//************ Begin File listing routine ************//
	
	StringVector svFileList;
	gs->filesystem->getDirectoryListing(svFileList); // sv contains the names of the files in \maps directory.

	StringVector svMapDimensions;
	gs->filesystem->getMapDimensions(svMapDimensions);

	// Save these Map file name properties first.

	std::string fileinfo;
	cursor.Y = bd2.baseY + 5;
	
	for(size_t i = 0; i < svFileList.size(); ++i)
	{
		// Catenate filename and its dimensions.
		fileinfo = svFileList[i] + "            " + svMapDimensions[i];
		cursor.X = bd2.baseX + (bd2.width / 2) - (fileinfo.length() / 2);

		// Store these properties so the input device can refer to them.
		mMapFilesVector.push_back(std::make_tuple
									(i, cursor, 
									FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 
									fileinfo, 
									0, 
									true, 
									false));

		cursor.Y++;
	}

	// Now draw the file names.
	for (mItemTupleVectorIter = mMapFilesVector.begin(); mItemTupleVectorIter != mMapFilesVector.end(); ++mItemTupleVectorIter)
	{
		// Sets the string color arbitrarily defined by the input handler.
		SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(*mItemTupleVectorIter));
		SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorIter));
		std::cout << std::get<3>(*mItemTupleVectorIter);
	}

	//************ End File listing routine ************//


	// Position the cursor to the location of the underscore character.
	mItemTupleVectorIter = mItemTupleVector.begin() + 8;
	SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorIter));

	posX = bd3.baseX + 1;
}

// Function: Draws the FilenameView object.
void FilenameView::draw()
{
	
	// Draw the scrollbar and all other auxillary strings.
	/*for (mItemTupleVectorIter = mItemTupleVector.begin(); mItemTupleVectorIter != mItemTupleVector.end(); ++mItemTupleVectorIter)
	{
		// Sets the string color arbitrarily defined by the input handler.
		SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(*mItemTupleVectorIter));
		SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorIter));
		if(std::get<4>(*mItemTupleVectorIter) != 0)
		{
			_setmode(_fileno(stdout), _O_U16TEXT);
			std::wcout << std::get<4>(*mItemTupleVectorIter);
			_setmode(_fileno(stdout), _O_TEXT);
		}
		else
			std::cout << std::get<3>(*mItemTupleVectorIter);
	}
	*/

		for (mItemTupleVectorIter = mMapFilesVector.begin(); mItemTupleVectorIter != mMapFilesVector.end(); ++mItemTupleVectorIter)
	{
		// Sets the string color arbitrarily defined by the input handler.
		SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(*mItemTupleVectorIter));
		SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorIter));
		std::cout << std::get<3>(*mItemTupleVectorIter);
	}

	SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(mItemTupleVector.at(4)));
	SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(mItemTupleVector.at(4)));
	std::cout << std::get<3>(mItemTupleVector.at(4));

	SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(mItemTupleVector.at(5)));
	SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(mItemTupleVector.at(5)));
	std::cout << std::get<3>(mItemTupleVector.at(5));
	
	for (mItemTupleVectorIter = mMapFilesVector.begin(); mItemTupleVectorIter != mMapFilesVector.end(); ++mItemTupleVectorIter)
	{
		// Sets the string color arbitrarily defined by the input handler.
		SetConsoleTextAttribute(gs->console->getConsoleHandle(), std::get<2>(*mItemTupleVectorIter));
		SetConsoleCursorPosition(gs->console->getConsoleHandle(), std::get<1>(*mItemTupleVectorIter));
		std::cout << std::get<3>(*mItemTupleVectorIter);
	}

	SetConsoleTextAttribute(gs->console->getConsoleHandle(), wWhite);

	return;
}

void FilenameView::draw(std::string & s)
{
	// Clear the area of the file input box.
	clearFileInputBox();

	COORD coord;
	coord.X = posX;
	coord.Y = bd3.baseY + 1;

	SetConsoleCursorPosition(gs->console->getConsoleHandle(), coord);
	std::cout << s;
}

// Handle mouse events here.
void FilenameView::draw(WORD wParam)
{
	clearFileInputBox();

	draw(gs->filesystem->getFileName(wParam));
	return;
}

void FilenameView::clearFileInputBox()
{
	SMALL_RECT srRect;
	srRect.Top = bd3.baseY + 1;
	srRect.Left = bd3.baseX + 1;
	srRect.Bottom = bd3.baseY + 1;
	srRect.Right = bd3.baseX + bd3.width - 2;

	COORD cBase = { 0, 0 };
	COORD dwSize = { bd3.width, bd3.height };
	CHAR_INFO mRect[39];

	for(int i = 0; i < bd3.width; ++i)
	{
		mRect[i].Char.UnicodeChar = ' ';
		mRect[i].Attributes = 0x0F;
	}

	if(!WriteConsoleOutput(gs->console->getConsoleHandle(), mRect, dwSize, cBase, &srRect))
	{
		CLogger::instance().log("Unable to read input records in InputDeviceHandler::poll()",
										CLogger::kLogLevelDebug);
	}
}

std::string FilenameView::getFileNameSelected()
{
	std::string fname;

	for(ItemTupleVectorIter it = mMapFilesVector.begin(); it != mMapFilesVector.end(); ++ it)
	{
		if(std::get<6>(*it))
		{
			std::string str = std::get<3>(*it);
			std::stringstream ss(str);
			std::getline(ss, fname, ' ');
			break;
		}
	}

	return fname;
}
