//**************************************************************************************************//
// bounding_box_helper.h                                                                            //
//                                                                                                  //
// Definition: Implementation file for BoundingBoxHelper class                                      //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/21/2012 - Added comments.                                                                      //.
//                                                                                                  //
// Notes: This class is responsible for drawing the actual bounding box around the console window.  //
//        Each character in the box is represented by an extended ascii code.                       //
//**************************************************************************************************//

#include "bounding_box_helper.h"

// Default ctor.
BoundingBoxHelper::BoundingBoxHelper()
{ }

// Default dtor.
BoundingBoxHelper::~BoundingBoxHelper()
{ }

// Overloaded ctor.
BoundingBoxHelper::BoundingBoxHelper( TDConsole *console ){
	this->console = console;
}

// Function: Stores the coordinates of the console window's surrounding area (minus one character unit) into a vector.  
// It then returns that vector.
std::vector<cellObj> BoundingBoxHelper::getBoundingBox( int baseX, int baseY, int width, int height, bool sngl ) {
	
	std::vector<cellObj> cellObjVector;
	COORD dwCoord;

	try
	{
		// Upper-left corner.
		dwCoord.X = baseX;
		dwCoord.Y = baseY;
		cellObjVector.push_back ( std::make_pair ( dwCoord, ( sngl ? char(218) : char ( 201 ) ) ) );

		// Upper-right corner.
		dwCoord.X = baseX + width - 1;
		dwCoord.Y = baseY;
		cellObjVector.push_back ( std::make_pair ( dwCoord, ( sngl ? char(191) : char ( 187 ) ) ) );

		//// Lower-left corner.
		dwCoord.X = baseX;
		dwCoord.Y = baseY + height - 1;
		cellObjVector.push_back ( std::make_pair ( dwCoord, ( sngl ? char(192) : char ( 200 ) ) ) );

		//// Lower-right corner.
		dwCoord.X = baseX + width - 1;
		dwCoord.Y = baseY + height - 1;
		cellObjVector.push_back ( std::make_pair ( dwCoord, ( sngl ? char(217) : char ( 188 ) ) ) );

		//// Top-side bar line.
		for ( int i = 1; i < width - 1; ++i )
		{
			dwCoord.X = baseX + i;
			dwCoord.Y = baseY;
			cellObjVector.push_back ( std::make_pair ( dwCoord, ( sngl ? char(196) : char ( 205 ) ) ) );
		}

		//// Bottom-side bar line.
		for ( int i = 1; i < width - 1; ++i )
		{
			dwCoord.X = baseX + i;
			dwCoord.Y = baseY + height - 1;
			cellObjVector.push_back ( std::make_pair ( dwCoord, ( sngl ? char(196) : char ( 205 ) ) ) );
		}

		//// Left-side bar line.
		for ( int i = 1; i < height - 1; ++i )
		{
			dwCoord.X = baseX;
			dwCoord.Y = baseY + i;
			cellObjVector.push_back ( std::make_pair ( dwCoord, ( sngl ? char(179) : char ( 186 ) ) ) );
		}

		//// Right-side bar line.
		for ( int i = 1; i < height - 1; ++i )
		{
			dwCoord.X = baseX + width - 1;
			dwCoord.Y = baseY + i;
			cellObjVector.push_back ( std::make_pair ( dwCoord, ( sngl ? char(179) : char ( 186 ) ) ) );
		}
	}
	catch ( std::bad_alloc& )
	{
		CLogger::instance().log ( "Vector re-allocation failed in CGameOverView::draw().",
									CLogger::kLogLevelError );
		std::exit ( EXIT_FAILURE );
	}

	return cellObjVector;
}

// Function: Prints the contents of the vector containing coordinates and extended ascii characters.
void BoundingBoxHelper::drawBoundingBox( std::vector<cellObj> boxVector ) {

	for (cit = boxVector.begin(); cit != boxVector.end(); ++cit )
	{
		SetConsoleCursorPosition(console->getConsoleHandle(), cit->first);
		std::cout << cit->second;
	}
}

// Function: Prints an arbitrary sized bounding box.
void BoundingBoxHelper::drawBoundingBox( int baseX, int baseY, int width, int height, bool sngl ) {

	std::vector<cellObj> boxVector = getBoundingBox( baseX, baseY, width, height, sngl );

	for( cit = boxVector.begin ( ); cit != boxVector.end ( ); ++cit)
	{
		SetConsoleCursorPosition(console->getConsoleHandle(), cit->first);
		std::cout << cit->second;
	}
}
