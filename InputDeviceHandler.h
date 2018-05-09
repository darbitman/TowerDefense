//**************************************************************************************************//
// InputDeviceHandler.h                                                                             //
//                                                                                                  //
// Definition: Header file for InputDeviceHandler.cpp                                               //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/19/2012 - File created.                                                                        //
//                                                                                                  //
// Notes: This class handles both keyboard and mouse inputs. The primary function ReadConsoleInput  //
//        is not compatible with conio.h's _kbhit so that function has been disabled.               //
//**************************************************************************************************//

#ifndef INPUT_DEVICE_HANDLER_H
#define INPUT_DEVICE_HANDLER_H

#define MOUSE_HOVER		0
#define MOUSE_CLICK		1

#define SB_BUTTON_DOWN	2
#define SB_BUTTON_UP	3

#define BUTTON_LOAD		99
#define BUTTON_SAVE		100
#define BUTTON_CANCEL	101

class ViewManager;
class MainMenuView;
class GameOverView;
class StatusView;
class InfoPaneView;
class FilenameView;
class GameState;

#include "Config.h"
#include "view.h"
#include "Logger\Logger.h"

class InputDeviceHandler
{
public:
	InputDeviceHandler();
	~InputDeviceHandler();

	InputDeviceHandler(GameState *);
	void init();
	void poll();
	void revertConsole();

	// Keyboard state related member functions.
	bool isKeyPressed();
	char getKeyPressed() const;

	// Mouse button state related member functions.
	bool isLeftMouseButtonClicked();
	WORD getItemClicked();

	// Mouse event related member functions.
	bool isMouseHoveringOnItem();
	WORD getItemHovered();

	// This is where most of mouse input processing will be done.
	WORD processMouseInput(WORD);

	// Allow the poll function to accept mouse/keyboard input.
	void validateMouseInput();

	// Checks whether this class accepts mouse/keyboard input.
	bool inputDeviceIsValid() const;

	// Reset this class's original state.
	void reset();

	void setDialogBoxState(); // Hack to set the FilenameView as the current view.
	void unsetDialogBoxState();

private:
	// Disable default copy constructor and assignment operator.
	InputDeviceHandler(const InputDeviceHandler &);
	InputDeviceHandler & operator=(const InputDeviceHandler &);

	
private:

	bool			mKeyPressed, mLeftMouseButtonClicked, 
					mMousePointerIsHoveringOnItem,
					mMouseEventIsValid;
	bool			initialized;
	bool			dialogboxstate;

	HANDLE			hStdin;
	DWORD			fdwMode, fdwSaveOldMode, dwNumEventsRead;
	char			wKeyCode;
	WORD			wMouseCode;
	INPUT_RECORD	irEvent;

	GameState *		gs;
	ViewManager	*	mViewManager;

};

#endif
