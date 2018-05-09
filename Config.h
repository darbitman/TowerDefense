#ifndef Config_h__
#define Config_h__

// Global includes.
#include <iostream>
#include <fstream>
#include <memory>
#include <typeinfo>
#include <iomanip>
#include <vector>
#include <list>
#include <deque>
#include <tuple>
#include <utility>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>

#include <stdio.h>		// _fileno
#include <io.h>			// _setmode
#include <fcntl.h>		// _O_U16TEXT - unicode char

#include <process.h>	//_beginthreadex _endthreadex
#include <Windows.h>
#include <strsafe.h>	// StringCchCat, StringCchCopy

// Console window's height and width.
#define CONSOLE_WINDOW_HEIGHT	50
#define CONSOLE_WINDOW_WIDTH	80

// Object ID, Coordinates, text color attribute, string value | wide char value, string hoverable/clickable?, string currently selected?
typedef std::tuple<SHORT, COORD, WORD, std::string, wchar_t, bool, bool> ItemTuple;
typedef std::vector<ItemTuple>ItemTupleVector;
typedef std::vector<ItemTuple>::const_iterator ItemTupleVectorConstIter;
typedef std::vector<ItemTuple>::iterator ItemTupleVectorIter;

typedef std::vector<std::string> StringVector;

enum game_state{main_menu, edit_mode, build_mode, run_mode, game_over, map_editor};

using namespace std;

// Global typedefs.
typedef __int64 int64;
typedef __int32 int32;
typedef __int16 int16;
typedef __int8 int8;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;

#pragma warning(disable : 4309)   // 'static_cast' : truncation of constant value
#pragma warning(disable : 4244)		// double to SHORT warning message
#pragma warning(disable : 4018)		// signed/unsigned mismatch
#pragma warning(disable : 4715)		// not all path have return...
#pragma warning(disable : 4065)		// switch statement contains 'default' but no 'case' labels.

#endif // Config_h__