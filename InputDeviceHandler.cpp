//**************************************************************************************************//
// InputDeviceHandler.cpp                                                                           //
//                                                                                                  //
// Definition: Implementation for InputDeviceHandler.cpp                                            //
// Version 0.1                                                                                      //
//                                                                                                  //
// Changelog:                                                                                       //
// 5/19/2012 - File created.                                                                        //
//                                                                                                  //
// Notes: This class handles both keyboard and mouse inputs. The primary function ReadConsoleInput  //
//        is not compatible with conio.h's _kbhit so that function has been disabled.               //
//**************************************************************************************************//

#include "InputDeviceHandler.h"
#include "ViewManager.h"
#include "main_menu_view.h"
#include "status_view.h"
#include "info_pane_view.h"
#include "GameOverView.h"
#include "filename_view.h"
#include "game_state.h"

// Default constructor.
InputDeviceHandler::InputDeviceHandler()
{
	CLogger::instance().log("This class needs to be constructed with ViewManager parameter in InputDeviceHandler::InputDeviceHandler()",
								CLogger::kLogLevelError);
	return;
}

// Defautl destructor.
InputDeviceHandler::~InputDeviceHandler()
{
	SetConsoleMode (hStdin, fdwSaveOldMode);
}

InputDeviceHandler::InputDeviceHandler(GameState * gs)
{
	this->gs = gs;
	mViewManager = &gs->views;
	init();
}

// Initialization routines.
void InputDeviceHandler::init()
{
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	// Get the standard input handle.
	if (hStdin == INVALID_HANDLE_VALUE)
	{
		CLogger::instance().log ("Unable to acquire standard input handle in InputDeviceHandler::init()",
										CLogger::kLogLevelDebug);
		return;
	}

	// Save the current input mode.
	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
	{
		CLogger::instance().log ("Unable to save the current input mode in InpuDeviceHandler::init()",
										CLogger::kLogLevelDebug);
		return;
	}

	// Enable mouse input events.
	fdwMode = ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
	{
		CLogger::instance().log("Unable to set the new console mode in InputDeviceHandler::init()",
										CLogger::kLogLevelDebug);
		return;
	}

	mKeyPressed = mLeftMouseButtonClicked = mMousePointerIsHoveringOnItem = false;
	mMouseEventIsValid = true;
	initialized = false;
	dialogboxstate = false;
}

void InputDeviceHandler::poll()
{
	if(mMouseEventIsValid)
	{
		WaitForSingleObject(hStdin, INFINITE);
	
		// ReadConsoleInput blocks so we use PeekConsoleInput first.
		if(PeekConsoleInput(hStdin, &irEvent, 1, &dwNumEventsRead))
		{
			// Listen for keyboard events.
			if((irEvent.EventType == KEY_EVENT) && (irEvent.Event.KeyEvent.bKeyDown))
			{
				if(!ReadConsoleInput(hStdin, &irEvent, 1, &dwNumEventsRead))
				{
					CLogger::instance().log("Unable to read key event in InputDeviceHandler::poll()",
												CLogger::kLogLevelDebug);
				}

				mKeyPressed = true;
				wKeyCode = irEvent.Event.KeyEvent.uChar.UnicodeChar;
				
				if(wKeyCode == 0)
					wKeyCode = irEvent.Event.KeyEvent.wVirtualKeyCode;

			}
			
			// Listen for mouse events
			else if((irEvent.EventType == MOUSE_EVENT) && (irEvent.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED))
			{
				if(!ReadConsoleInput(hStdin, &irEvent, 1, &dwNumEventsRead))
				{
					CLogger::instance().log("Unable to read mouse event and button press records in InputDeviceHandler::poll()",
												CLogger::kLogLevelDebug);
				}

				mLeftMouseButtonClicked = true;

			}

			// Assume mouse cursor is hovering on selected item.
			else if((irEvent.EventType == MOUSE_EVENT) && (irEvent.Event.MouseEvent.dwEventFlags == MOUSE_MOVED))
			{
				if(!ReadConsoleInput(hStdin, &irEvent, 1, &dwNumEventsRead))
				{
					CLogger::instance().log("Unable to read mouse event records in InputDeviceHandler::poll()",
												CLogger::kLogLevelDebug);
				}

				mMousePointerIsHoveringOnItem = true;
			}
		}

		FlushConsoleInputBuffer(hStdin);
	}
}

void InputDeviceHandler::revertConsole()
{
	SetConsoleMode (hStdin, fdwSaveOldMode);
}


//********************* Keyboard state related member functions *********************//

bool InputDeviceHandler::isKeyPressed()
{
	if(mKeyPressed)
	{
		mKeyPressed = false;
		return true;
	}

	return false;
}

char InputDeviceHandler::getKeyPressed() const
{
	return wKeyCode;
}

//********************* End Keyboard state related member functions *********************//




//********************* Mouse event related member functions *********************//

bool InputDeviceHandler::isLeftMouseButtonClicked()
{
	if(mLeftMouseButtonClicked)
	{
		// Reset left mouse button state before returning.
		mLeftMouseButtonClicked = false;
		return true;
	}

	return false;
}

// Function will process the string or unicode char object associated
// with the current active view.
WORD InputDeviceHandler::getItemClicked()
{
	if(!mViewManager->views.empty())
	{
		return processMouseInput(MOUSE_CLICK);
	}

	return 0;
}

bool InputDeviceHandler::isMouseHoveringOnItem()
{
	if(mMousePointerIsHoveringOnItem)
	{
		// Reset mouse hovering state before returning.
		mMousePointerIsHoveringOnItem = false;
		return true;
	}

	return false;
}

WORD InputDeviceHandler::getItemHovered()
{
	if(!mViewManager->views.empty())
	{
		return processMouseInput(MOUSE_HOVER);
	}
	
	return 0;
}



// Parameter w: 0 = click, 1 = hover
WORD InputDeviceHandler::processMouseInput(WORD wParam)
{
	if(gs->gs == main_menu)
	{
		if(dialogboxstate == true)
		{
			// Check if we have the open/save dialog box is currently the active view.
			for(ViewIter vt = mViewManager->views.begin(); vt != mViewManager->views.end(); ++vt)
			{
				if(typeid(**vt) == typeid(FilenameView))
				{
					std::auto_ptr<FilenameView> fnv = std::auto_ptr<FilenameView>(dynamic_cast<FilenameView *>(*vt));

					// This for loop is for listening events for the Map files.
					for(ItemTupleVectorIter itvit = fnv->mMapFilesVector.begin(); itvit != fnv->mMapFilesVector.end(); ++itvit)
					{
						// Check if current string is clickable...
						if(!std::get<5>(*itvit))
							continue;

						// Check if mouse pointer falls within the boundaries of selected string.
						else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
									((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
									(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
						{

							// If so, check if we are processing a mouse click or mouse hover, then return appropriate value.
							if(wParam == MOUSE_CLICK)
							{
								// Draw file/save dialog box with filename on the input box.
								fnv->draw(std::get<0>(*itvit));

								// Find which item has been selected before so we can reset its state
								for(ItemTupleVectorIter it = fnv->mMapFilesVector.begin(); it != fnv->mMapFilesVector.end(); ++it)
								{
									if(std::get<6>(*it) == true)
										std::get<6>(*it) = false;
								}

								// Set the current state of this newly selected item to true;
								std::get<6>(*itvit) = true;

								// Destroy this auto_ptr but not the object it points to.
								fnv.release();

								// Get the id of this file name.
								return std::get<0>(*itvit);
							}
							else if(wParam == MOUSE_HOVER && dialogboxstate == true)
							{
								// Draw file/save dialog box with filename on the input box.
								fnv->draw(std::get<0>(*itvit));

								// Find the previously selected item and change its "selected" state to no longer selected.
								for(ItemTupleVectorIter it = fnv->mMapFilesVector.begin(); it != fnv->mMapFilesVector.end(); ++it)
								{
									if(std::get<6>(*it) == true)
									{
										std::get<2>(*it) = 0x0F;
										std::get<6>(*it) = false;
									}
								}
						
								// Change the "is currently selected" state of this currently selected item.
								std::get<6>(*itvit) = true;

								// Update the background color of currently selected item.
								std::get<2>(*itvit) = 0x70;
							
								fnv->draw();
								fnv.release();

								return 0;
							}
						}
					}

					// This for loop is for listening events for the auxillary strings in the FilenameView eg. Save, Load, Cancel
					for(ItemTupleVectorIter itvit = fnv->mItemTupleVector.begin(); itvit != fnv->mItemTupleVector.end(); ++itvit)
					{
						// Check if current string is clickable...
						if(!std::get<5>(*itvit))
							continue;

						// Check if mouse pointer falls within the boundaries of selected string.
						else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
									((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
									(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
						{
							//mMouseEventIsValid = false;

							// If so, check if we are processing a mouse click or mouse hover, then return appropriate value.
							if(wParam == MOUSE_CLICK)
							{
								//fnv->draw(std::get<0>(*itvit));
								switch(std::get<0>(*itvit))
								{
									case 99		:	fnv.release();
													return BUTTON_LOAD;
									case 100	:	fnv.release();
													return BUTTON_SAVE;
									case 101	:	fnv.release();
													return BUTTON_CANCEL;
								}
							}
							else if(wParam == MOUSE_HOVER && dialogboxstate == true)
							{
								mMouseEventIsValid = true;

								// Find the previously selected item and change its "selected" state to no longer selected.
								for(ItemTupleVectorIter it = fnv->mItemTupleVector.begin(); it != fnv->mItemTupleVector.end(); ++it)
								{
									if(std::get<6>(*it) == true)
									{
										std::get<2>(*it) = 0x0F;
										std::get<6>(*it) = false;
									}
								}
						
								// Change the "is currently selected" state of this currently selected item.
								std::get<6>(*itvit) = true;

								// Update the background color of currently selected item.
								std::get<2>(*itvit) = 0x70;
							
								fnv->draw();
								fnv.release();

								return 0;

							}
						}
					}

					fnv.release();

				} // Outer if

			}
		}
		else
		{
			std::auto_ptr<MainMenuView> mmv = std::auto_ptr<MainMenuView>(new MainMenuView(gs));

			// Loop through the MainMenuView's vector of tuples.
			for(ItemTupleVectorIter itvit = mmv->mItemTupleVector.begin(); itvit != mmv->mItemTupleVector.end(); ++itvit)
			{
				// Check if current string is clickable...
				if(!std::get<5>(*itvit))
					continue;

				// Check if mouse pointer falls within the boundaries of selected string.
				else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
							((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
							(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
				{
					// If so, check if we are processing a mouse click or mouse hover, then return appropriate value as a mouse click event.
					if(wParam == MOUSE_CLICK)
					{
						mmv.release();
						return std::get<0>(*itvit);
					}
					else if(wParam == MOUSE_HOVER) // Process mouse hovering event on the item that is currently selected.
					{
					
						mMouseEventIsValid = true;

						// Find the previously selected item and change its "selected" state to no longer selected.
						for(ItemTupleVectorIter it = mmv->mItemTupleVector.begin(); it != mmv->mItemTupleVector.end(); ++it)
						{
							if(std::get<6>(*it) == true)
							{
								std::get<2>(*it) = 0x0F;
								std::get<6>(*it) = false;
							}
						}
						
						// Change the "is currently selected" state of this currently selected item.
						std::get<6>(*itvit) = true;

						// Update the background color of currently selected item.
						std::get<2>(*itvit) = 0x70;
							
						mmv->draw();
						mmv.release();

						return 0;
					}
				} // inner else if
			} // Outer for

			mmv.release();
		}
	}

	else if(gs->gs == edit_mode)
	{
		std::auto_ptr<InfoPaneView> ipv = std::auto_ptr<InfoPaneView>(new InfoPaneView(gs));

		// Handle Mouse events in Edit Control Command strings here..
		for(ItemTupleVectorIter itvit = ipv->mEditControlCommands.begin(); itvit != ipv->mEditControlCommands.end(); ++itvit)
		{
			if(!std::get<5>(*itvit))
				continue;

			// Check if mouse pointer falls within the boundaries of selected string.
			else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
						((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
						(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
			{
				if(wParam == MOUSE_CLICK)
				{
					return std::get<0>(*itvit);
				}
				else if(wParam == MOUSE_HOVER)
				{
					mMouseEventIsValid = true;

					// Find the previously selected item and change its "selected" state to no longer selected.
					for(ItemTupleVectorIter it = ipv->mEditControlCommands.begin(); it != ipv->mEditControlCommands.end(); ++it)
					{
						if(std::get<6>(*it) == true)
						{
							std::get<2>(*it) = 0x0F;
							std::get<6>(*it) = false;
						}
					}
						
					// Change the "is currently selected" state of this currently selected item.
					std::get<6>(*itvit) = true;

					// Update the background color of currently selected item.
					std::get<2>(*itvit) = 0x70;
							
					ipv->draw();
					ipv.release();

					return 0;
				}
			}
		} // End For loop for Edit Control Command

		// Handle Mouse events in Build Command strings here..
		for(ItemTupleVectorIter itvit = ipv->mBuildCommands.begin(); itvit != ipv->mBuildCommands.end(); ++itvit)
		{
			if(!std::get<5>(*itvit))
				continue;

			// Check if mouse pointer falls within the boundaries of selected string.
			else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
						((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
						(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
			{
				if(wParam == MOUSE_CLICK)
				{
					return std::get<0>(*itvit);
				}
				else if(wParam == MOUSE_HOVER)
				{
					mMouseEventIsValid = true;

					// Find the previously selected item and change its "selected" state to no longer selected.
					for(ItemTupleVectorIter it = ipv->mBuildCommands.begin(); it != ipv->mBuildCommands.end(); ++it)
					{
						if(std::get<6>(*it) == true)
						{
							std::get<2>(*it) = 0x0F;
							std::get<6>(*it) = false;
						}
					}
						
					// Change the "is currently selected" state of this currently selected item.
					std::get<6>(*itvit) = true;

					// Update the background color of currently selected item.
					std::get<2>(*itvit) = 0x70;
							
					ipv->draw();
					ipv.release();

					return 0;
				}
			}
		} // End For loop for Build Command

		// Handle Mouse events in StatusView scrollbar.
		for(ViewIter vt = mViewManager->views.begin(); vt != mViewManager->views.end(); ++vt)
		{
			if(typeid(**vt) == typeid(StatusView))
			{
				std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(dynamic_cast<StatusView *>(*vt));

				// This for loop is for listening events in the StatusView
				for(ItemTupleVectorIter itvit = sv->mItemTupleVector.begin(); itvit != sv->mItemTupleVector.end(); ++itvit)
				{
					// Check if current unicode character selected in scrollbar is clickable.
					if(!std::get<5>(*itvit))
						continue;

					// Scrollbar's up arrow has been selected.
					if((irEvent.Event.MouseEvent.dwMousePosition.X == std::get<1>(sv->mItemTupleVector.front()).X) &&
						(irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(sv->mItemTupleVector.front()).Y))
					{
						// Process mouse click event.
						if(wParam == MOUSE_CLICK)
						{
							sv->draw(SB_BUTTON_UP);
							sv.release();
							return 0;
						}
						else if(wParam == MOUSE_HOVER && dialogboxstate != true)
						{
							sv.release();
							return 0;
						}

					}
					// Scrollbar's down arrow has been selected.
					else if((irEvent.Event.MouseEvent.dwMousePosition.X == std::get<1>(sv->mItemTupleVector.back()).X) &&
							(irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(sv->mItemTupleVector.back()).Y))
					{
						if(wParam == MOUSE_CLICK)
						{
							sv->draw(SB_BUTTON_DOWN);
							sv.release();
							return 0;
						}
						else if(wParam == MOUSE_HOVER && dialogboxstate != true)
						{
							sv.release();
							return 0;
						}
					}
				} // Inner for loop

				sv.release();
			}
		} // End for loop for scrollbar.

	} // End else if

	else if(gs->gs == map_editor)
	{

		std::auto_ptr<InfoPaneView> ipv = std::auto_ptr<InfoPaneView>(new InfoPaneView(gs));

		// Handle Mouse events in InfoPaneView Command strings here..
		for(ItemTupleVectorIter itvit = ipv->mMapEditorCommands.begin(); itvit != ipv->mMapEditorCommands.end(); ++itvit)
		{
			// Check if current string selected is clickable.
			if(!std::get<5>(*itvit))
				continue;

			// Check if mouse pointer falls within the boundaries of selected string.
			else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
						((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
						(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
			{
				// Process mouse click event.
				if(wParam == MOUSE_CLICK)
				{
					// Find which item has been selected before so we can reset its state
					for(ItemTupleVectorIter it = ipv->mMapEditorCommands.begin(); it != ipv->mMapEditorCommands.end(); ++it)
					{
						if(std::get<6>(*it) == true)
							std::get<6>(*it) = false;
					}

					// Set the current state of this newly selected item to true;
					std::get<6>(*itvit) = true;

					// Destroy this auto_ptr but not the object it points to.
					ipv.release();

					return std::get<0>(*itvit);
				}
				else if(wParam == MOUSE_HOVER && dialogboxstate != true)
				{
					mMouseEventIsValid = true;

					// Find the previously selected item and change its "selected" state to no longer selected.
					for(ItemTupleVectorIter it = ipv->mMapEditorCommands.begin(); it != ipv->mMapEditorCommands.end(); ++it)
					{
						if(std::get<6>(*it) == true)
						{
							std::get<2>(*it) = 0x0F;
							std::get<6>(*it) = false;
						}
					}
						
					// Change the "is currently selected" state of this currently selected item.
					std::get<6>(*itvit) = true;

					// Update the background color of currently selected item.
					std::get<2>(*itvit) = 0x70;
							
					ipv->draw();
					ipv.release();

					return 0;
				}
			}
		} // End For loop for Map Editor Command

		// Don't forget to destroy this auto_ptr.
		ipv.release();


		// Handle Mouse events in StatusView scrollbar.
		for(ViewIter vt = mViewManager->views.begin(); vt != mViewManager->views.end(); ++vt)
		{
			if(typeid(**vt) == typeid(StatusView))
			{
				std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(dynamic_cast<StatusView *>(*vt));

				// This for loop is for listening events in the StatusView
				for(ItemTupleVectorIter itvit = sv->mItemTupleVector.begin(); itvit != sv->mItemTupleVector.end(); ++itvit)
				{
					// Check if current unicode character selected in scrollbar is clickable.
					if(!std::get<5>(*itvit))
						continue;

					// Scrollbar's up arrow has been selected.
					if((irEvent.Event.MouseEvent.dwMousePosition.X == std::get<1>(sv->mItemTupleVector.front()).X) &&
						(irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(sv->mItemTupleVector.front()).Y))
					{
						// Process mouse click event.
						if(wParam == MOUSE_CLICK)
						{
							sv->draw(SB_BUTTON_UP);
							sv.release();
							return 0;
						}
						else if(wParam == MOUSE_HOVER && dialogboxstate != true)
						{
							sv.release();
							return 0;
						}

					}
					// Scrollbar's down arrow has been selected.
					else if((irEvent.Event.MouseEvent.dwMousePosition.X == std::get<1>(sv->mItemTupleVector.back()).X) &&
							(irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(sv->mItemTupleVector.back()).Y))
					{
						if(wParam == MOUSE_CLICK)
						{
							sv->draw(SB_BUTTON_DOWN);
							sv.release();
							return 0;
						}
						else if(wParam == MOUSE_HOVER && dialogboxstate != true)
						{
							sv.release();
							return 0;
						}
					}
				} // Inner for loop

				sv.release();
			}

		} // Outer for loop

		// Check if we have the open/save dialog box is currently the active view.
		for(ViewIter vt = mViewManager->views.begin(); vt != mViewManager->views.end(); ++vt)
		{
			if(typeid(**vt) == typeid(FilenameView))
			{
				std::auto_ptr<FilenameView> fnv = std::auto_ptr<FilenameView>(dynamic_cast<FilenameView *>(*vt));

				// This for loop is for listening events for the Map files.
				for(ItemTupleVectorIter itvit = fnv->mMapFilesVector.begin(); itvit != fnv->mMapFilesVector.end(); ++itvit)
				{
					// Check if current string is clickable...
					if(!std::get<5>(*itvit))
						continue;

					// Check if mouse pointer falls within the boundaries of selected string.
					else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
								((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
								(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
					{

						// If so, check if we are processing a mouse click or mouse hover, then return appropriate value.
						if(wParam == MOUSE_CLICK)
						{
							// Draw file/save dialog box with filename on the input box.
							fnv->draw(std::get<0>(*itvit));

							// Find which item has been selected before so we can reset its state
							for(ItemTupleVectorIter it = fnv->mMapFilesVector.begin(); it != fnv->mMapFilesVector.end(); ++it)
							{
								if(std::get<6>(*it) == true)
									std::get<6>(*it) = false;
							}

							// Set the current state of this newly selected item to true;
							std::get<6>(*itvit) = true;

							// Destroy this auto_ptr but not the object it points to.
							fnv.release();

							// Get the id of this file name.
							return std::get<0>(*itvit);
						}
						else if(wParam == MOUSE_HOVER && dialogboxstate == true)
						{
							// Draw file/save dialog box with filename on the input box.
							fnv->draw(std::get<0>(*itvit));

							// Find the previously selected item and change its "selected" state to no longer selected.
							for(ItemTupleVectorIter it = fnv->mMapFilesVector.begin(); it != fnv->mMapFilesVector.end(); ++it)
							{
								if(std::get<6>(*it) == true)
								{
									std::get<2>(*it) = 0x0F;
									std::get<6>(*it) = false;
								}
							}
						
							// Change the "is currently selected" state of this currently selected item.
							std::get<6>(*itvit) = true;

							// Update the background color of currently selected item.
							std::get<2>(*itvit) = 0x70;
							
							fnv->draw();
							fnv.release();

							return 0;
						}
					}
				}

				// This for loop is for listening events for the auxillary strings in the FilenameView eg. Save, Load, Cancel
				for(ItemTupleVectorIter itvit = fnv->mItemTupleVector.begin(); itvit != fnv->mItemTupleVector.end(); ++itvit)
				{
					// Check if current string is clickable...
					if(!std::get<5>(*itvit))
						continue;

					// Check if mouse pointer falls within the boundaries of selected string.
					else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
								((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
								(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
					{
						//mMouseEventIsValid = false;

						// If so, check if we are processing a mouse click or mouse hover, then return appropriate value.
						if(wParam == MOUSE_CLICK)
						{
							//fnv->draw(std::get<0>(*itvit));
							switch(std::get<0>(*itvit))
							{
								case 99		:	fnv->draw();
												fnv.release();
												return BUTTON_LOAD;
								case 100	:	fnv->draw();
												fnv.release();
												return BUTTON_SAVE;
								case 101	:	fnv.release();
												return BUTTON_CANCEL;
							}
						}
						else if(wParam == MOUSE_HOVER && dialogboxstate == true)
						{
							mMouseEventIsValid = true;

							// Find the previously selected item and change its "selected" state to no longer selected.
							for(ItemTupleVectorIter it = fnv->mItemTupleVector.begin(); it != fnv->mItemTupleVector.end(); ++it)
							{
								if(std::get<6>(*it) == true)
								{
									std::get<2>(*it) = 0x0F;
									std::get<6>(*it) = false;
								}
							}
						
							// Change the "is currently selected" state of this currently selected item.
							std::get<6>(*itvit) = true;

							// Update the background color of currently selected item.
							std::get<2>(*itvit) = 0x70;
							
							fnv->draw();
							fnv.release();

							return 0;

						}
					}
				}

				fnv.release();

			} // Outer if

		}

	} // End else if(gs->gs == map_editor)

	// Build mode input events
	else if(gs->gs == build_mode)
	{
		std::auto_ptr<InfoPaneView> ipv = std::auto_ptr<InfoPaneView>(new InfoPaneView(gs));

		for(ItemTupleVectorIter itvit = ipv->mBuildCommands.begin(); itvit != ipv->mBuildCommands.end(); ++itvit)
		{
			// Check if current character or string is clickable...
			if(!std::get<5>(*itvit))
				continue;

			// Check if mouse pointer falls within the boundaries of selected string.
			else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
						((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
						(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
			{
				// If so, check if we are processing a mouse click or mouse hover, then return appropriate value as a mouse click event.
				if(wParam == MOUSE_CLICK)
				{
					// Find which item has been selected before so we can reset its state
					for(ItemTupleVectorIter it = ipv->mBuildCommands.begin(); it != ipv->mBuildCommands.end(); ++it)
					{
						if(std::get<6>(*it) == true)
							std::get<6>(*it) = false;
					}

					// Set the current state of this newly selected item to true;
					std::get<6>(*itvit) = true;

					// Get the ID of this tuple object.
					switch(std::get<0>(*itvit))
					{
						case 0	:	ipv->draw(1);
									ipv.release();
									return 0;

						case 1	:	ipv->draw(1);
									ipv.release();
									return 9;  // Tab

						case 2	:	ipv->draw(1);
									ipv.release();
									return 13; // Return

						case 3	:	ipv->draw(1);
									ipv.release();
									return 46; // Delete

						case 4	:	ipv->draw(1);
									ipv.release();
									return 27; // Escape

						default :	ipv->draw(1);;
									ipv.release();
									return 0;
					}

				}
				else
				{
					// Process Mouse hovering logic.
					// Allow highlight selection of strings depending of current game state.

					// Find the previously selected item and change its "selected" state to no longer selected.
					for(ItemTupleVectorIter it = ipv->mBuildCommands.begin(); it != ipv->mBuildCommands.end(); ++it)
					{
						if(std::get<6>(*it) == true)
						{
							std::get<2>(*it) = 0x0F;
							std::get<6>(*it) = false;
						}
					}
						
					// Change the "is currently selected" state of this currently selected item.
					std::get<6>(*itvit) = true;

					// Update the background color of currently selected item.
					std::get<2>(*itvit) = 0x70;
							
					ipv->draw();
					ipv.release();

					return 0;
				}
			}
		}

		// Handle Mouse events in StatusView scrollbar.
		for(ViewIter vt = mViewManager->views.begin(); vt != mViewManager->views.end(); ++vt)
		{
			if(typeid(**vt) == typeid(StatusView))
			{
				std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(dynamic_cast<StatusView *>(*vt));

				// This for loop is for listening events in the StatusView
				for(ItemTupleVectorIter itvit = sv->mItemTupleVector.begin(); itvit != sv->mItemTupleVector.end(); ++itvit)
				{
					// Check if current unicode character selected in scrollbar is clickable.
					if(!std::get<5>(*itvit))
						continue;

					// Scrollbar's up arrow has been selected.
					if((irEvent.Event.MouseEvent.dwMousePosition.X == std::get<1>(sv->mItemTupleVector.front()).X) &&
						(irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(sv->mItemTupleVector.front()).Y))
					{
						// Process mouse click event.
						if(wParam == MOUSE_CLICK)
						{
							sv->draw(SB_BUTTON_UP);
							sv.release();
							return 0;
						}
						else if(wParam == MOUSE_HOVER && dialogboxstate != true)
						{
							sv.release();
							return 0;
						}

					}
					// Scrollbar's down arrow has been selected.
					else if((irEvent.Event.MouseEvent.dwMousePosition.X == std::get<1>(sv->mItemTupleVector.back()).X) &&
							(irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(sv->mItemTupleVector.back()).Y))
					{
						if(wParam == MOUSE_CLICK)
						{
							sv->draw(SB_BUTTON_DOWN);
							sv.release();
							return 0;
						}
						else if(wParam == MOUSE_HOVER && dialogboxstate != true)
						{
							sv.release();
							return 0;
						}
					}
				} // Inner for loop

				sv.release();
			}
		}

		ipv.release();
	} // End else if(gs->gs == build_mode)

	// Game Over input events.
	else if(gs->gs == game_over)
	{
		std::auto_ptr<GameOverView> gov = std::auto_ptr<GameOverView>(new GameOverView(gs));

		for(ItemTupleVectorIter itvit = gov->mItemTupleVector.begin(); itvit != gov->mItemTupleVector.end(); ++itvit)
		{
			// Check if current string is clickable...
			if(!std::get<5>(*itvit))
				continue;

			// Check if mouse pointer falls within the boundaries of selected string.
			else if((irEvent.Event.MouseEvent.dwMousePosition.Y == std::get<1>(*itvit).Y &&
						((irEvent.Event.MouseEvent.dwMousePosition.X >= std::get<1>(*itvit).X) &&
						(irEvent.Event.MouseEvent.dwMousePosition.X <= (std::get<1>(*itvit).X + std::get<3>(*itvit).length() )))))
			{
					
				mMouseEventIsValid = false;

				// If so, check if we are processing a mouse click or mouse hover, then return appropriate value.
				if(wParam == MOUSE_CLICK)
				{
					switch(std::get<0>(*itvit))
					{
						case 0	:	
						case 1	:	
						case 2	:	
						case 3	:	
						case 4	:	return 0;
						case 5	:	return 13;
						default	:	return 0;
					}
				}
				else
				{
					// Process mouse hovering.
					return 0;
				}
			}
			else
				continue;
		}

	} // End else if(gs->gs == game_over)
}

//********************* End Mouse event related member functions *********************//

void InputDeviceHandler::validateMouseInput()
{
	mMouseEventIsValid = true;
}

bool InputDeviceHandler::inputDeviceIsValid() const
{
	return mMouseEventIsValid;
}

void InputDeviceHandler::reset()
{
	SetConsoleMode(hStdin, fdwSaveOldMode);
	init();
}

void InputDeviceHandler::setDialogBoxState() // Hack to set the FilenameView as the current view.
{
	dialogboxstate = true;
}

void InputDeviceHandler::unsetDialogBoxState()
{
	dialogboxstate = false;
}