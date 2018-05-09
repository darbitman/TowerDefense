#include "Game.h"
#include "cursor.h"

Game::Game() {
	gs = new GameState(this);
	inputdevice = new InputDeviceHandler(gs);
	m_change = true;
	m_restart_timer = true;
	run_once = false;
	lives_count = gs->player->get_lives();
}

Game::~Game()
{
	delete gs;
	delete inputdevice;
}

void Game::run() {

	main_menu_init();
	uint32 startTime;

	while ( true ) {

		if( m_restart_timer ) {
			startTime = timeGetTime();
			m_restart_timer = false;
		}

		uint32 deltaTime = timeGetTime() - startTime;

		switch( gs->gs ) {
			case main_menu:
				main_menu_run();
				break;
			case map_editor:
				map_editor_run();
				break;
			case edit_mode:
				edit_mode_run();
				break;
			case build_mode:
				build_mode_run();
				break;
			case run_mode:
				run_mode_run( deltaTime );
				break;
		}
  }
}

void Game::main_menu_init() {
	gs->gs = main_menu;
	MainMenuView *mmv = new MainMenuView( gs );
	BoundingBox *bb = new BoundingBox( gs->console );
	gs->views.push_back( bb );
	gs->views.push_back( mmv );
}

void Game::main_menu_run() {

	if(!run_once)
	{
		run_once = true;
		draw_all_views();
	};

	while(true)
	{
		if(inputdevice->inputDeviceIsValid())
			inputdevice->poll();

		if(inputdevice->isKeyPressed())
		{
			switch(inputdevice->getKeyPressed())
			{
				case 13		:	leave_main_menu();
								play_init();
								m_change = true;
								break;

				case 'c'	:	leave_main_menu();
								loadFileDialogBox(LOAD_MAP);
								inputdevice->unsetDialogBoxState();
								run_once = false;
								main_menu_init();
								break;

				case 'e'	:	leave_main_menu();
								map_editor_init();
								m_change = true;
								break;

				case 'h'	:	leave_main_menu();
								loadHelpView();
								run_once = false;
								main_menu_init();
								m_change = true;
								gs->console->clearConsole();
								break;

				case 'a'	:	leave_main_menu();
								loadAboutView();
								run_once = false;
								main_menu_init();
								m_change = true;
								gs->console->clearConsole();
								break;

				case 27		:	std::exit(1);

				default		:	continue;
			}
		}

		else if(inputdevice->isLeftMouseButtonClicked())
		{
			switch(inputdevice->getItemClicked())
			{
				case 1		:	leave_main_menu();
								play_init();
								m_change = true;
								break;
				
				case 2		:   leave_main_menu();	
								loadFileDialogBox(LOAD_MAP);
								inputdevice->unsetDialogBoxState();
								run_once = false;
								main_menu_init();
								break;

				case 3		:	leave_main_menu();
								map_editor_init();
								m_change = true;
								break;

				case 4		:	leave_main_menu();
								loadHelpView();
								run_once = false;
								main_menu_init();
								m_change = true;
								gs->console->clearConsole();
								break;

				case 5		:	leave_main_menu();
								loadAboutView();
								run_once = false;
								main_menu_init();
								m_change = true;
								gs->console->clearConsole();
								break;

				case 6		:	std::exit(1);

				default		:	continue;
			}
		}

		else if(inputdevice->isMouseHoveringOnItem())
		{
			//switch(inputdevice->getItemHovered())
			//{
				// Do nothing on mouse hover event, let the inputdevice object handle it.
			//	default		:	break;
			//}
		}

		return;
	}
}

void Game::leave_main_menu() {
	
	gs->views.clear();
	run_once = false;
	gs->console->clearConsole();
	//inputdevice->reset();

}

void Game::map_editor_run() {

	if(!run_once)
	{
		run_once = true;
		draw_all_views();
	};

	inputdevice->validateMouseInput();

	while(true)
	{
		if(inputdevice->inputDeviceIsValid())
			inputdevice->poll();

		Coord xy;
		Coord oldxy;

		xy = gs->cursor->get_coord();  // Get current location of cursor.
		oldxy = xy;

		if(inputdevice->isKeyPressed()) 
		{
			switch(inputdevice->getKeyPressed())
			{
				// Left arrow
				case 37			:	if(xy.x > 0)
									{
										xy.x = xy.x - 1;
										update_cursor(xy, oldxy);
										m_change = true;
										std::auto_ptr<MapView> mv = std::auto_ptr<MapView>(new MapView(gs));
										gs->views.drawSingleView(mv.get());
									}
									return;

				// Right arrow
				case 39			:	if(xy.x < gs->map->getWidth() - 1)
									{
										xy.x = xy.x + 1;
										update_cursor(xy, oldxy);
										m_change = true;
										std::auto_ptr<MapView> mv = std::auto_ptr<MapView>(new MapView(gs));
										gs->views.drawSingleView(mv.get());
									}
									return;

				// Up arrow
				case 38			:	if(xy.y > 0)
									{
										xy.y = xy.y - 1;
										update_cursor(xy, oldxy);
										m_change = true;
										std::auto_ptr<MapView> mv = std::auto_ptr<MapView>(new MapView(gs));
										gs->views.drawSingleView(mv.get());
									}
									return;

				// Down arrow
				case 40			:	if(xy.y < gs->map->getHeight() - 1)
									{
										xy.y = xy.y + 1;
										update_cursor(xy, oldxy);
										m_change = true;
										std::auto_ptr<MapView> mv = std::auto_ptr<MapView>(new MapView(gs));
										gs->views.drawSingleView(mv.get());
									}
									return;

				case 'l'		:	{
										std::auto_ptr<FilenameView> fnv = std::auto_ptr<FilenameView>(new FilenameView(gs, LOAD_MAP));
										gs->views.push_back(fnv.get());
										m_change = true;
									
										std::string filename;

										while(true)
										{
											if(inputdevice->inputDeviceIsValid())
												inputdevice->poll();

											if(inputdevice->isKeyPressed())
											{
												char ret = inputdevice->getKeyPressed();

												if(filename.length() > 6) // Maximum of six characters for the length of the map file name.
												{
													if(ret == 13) // Carriage return
														return;
													else if(ret == 8) // Backspace
													{
														filename.pop_back();
														fnv->draw(filename);
														continue;
													}
													else
														continue;
												}

												if(ret == 13) // Carriage return
												{
													if(filename.empty())
														continue;
													else
													{
														// Do some file name sanity checks.
													}
												}
												else if(ret == 27) // Escape
												{
													filename.clear();
													break;
												}
												else if(ret == 8) // Backspace
												{
													if(!filename.empty())
														filename.pop_back();
													else
														continue;
												}
												else if((ret >= '0' && ret <= '9') || (ret >= 'a' && ret <= 'z') || (ret >= 'A' && ret <= 'Z'))
													filename.push_back(ret);
												else
													continue;

												fnv->draw(filename);
												ret = 0;

											} // Inner if
											else if(inputdevice->isLeftMouseButtonClicked())
											{
												SHORT ret = inputdevice->getItemClicked();

												// Locate which item in the FilenameView has been selected.
												filename = fnv->getFileNameSelected();

												switch(ret)
												{
													// The "Load" button was clicked.
													case 99		:	if(filename.empty())
																		continue;
																	else
																	{
																		filename = "maps/" + filename;
																		gs->switchMap(filename);
																		gs->views.pop_back();
																		gs->console->clearConsole();
																		gs->status_view->addMessage(filename + " map loaded");
																		draw_all_views();
																		inputdevice->reset();
																		m_change = true;
																	}
																	fnv.release();
																	return;

													// The "Cancel button was clicked.
													case 101	:	gs->views.pop_back();
																	fnv.release();
																	return;

													default		:	continue;

												}
											}
											else if(inputdevice->isMouseHoveringOnItem())
											{

											}
										} // Inner while
									}
									return;

				case 's'	:	{	
									std::auto_ptr<FilenameView> fnv = std::auto_ptr<FilenameView>(new FilenameView(gs, SAVE_MAP));
									gs->views.push_back(fnv.get());
									m_change = true;

									std::string filename;

										while(true)
										{
											if(inputdevice->inputDeviceIsValid())
												inputdevice->poll();

											if(inputdevice->isKeyPressed())
											{
												char ret = inputdevice->getKeyPressed();

												if(filename.length() > 6) // Maximum of six characters for the length of the map file name.
												{
													if(ret == 13) // Carriage return
														return;
													else if(ret == 8) // Backspace
													{
														filename.pop_back();
														fnv->draw(filename);
														continue;
													}
													else
														continue;
												}

												if(ret == 13) // Carriage return
												{
													if(filename.empty())
														continue;
													else if(gs->filesystem->fileIsEqual(filename + ".tdm"))
														continue;
													else
													{
														filename = "maps/" + filename + ".tdm";
														gs->map->saveMap(filename);
														gs->views.pop_back();
														gs->console->clearConsole();
														gs->status_view->addMessage(filename + " map saved");
														inputdevice->reset();
														gs->filesystem->readDirectory();
														draw_all_views();
														m_change = true;
													}

													fnv.release();
													return;

												}
												else if(ret == 27) // Escape
												{
													filename.clear();
													break;
												}
												else if(ret == 8) // Backspace
												{
													if(!filename.empty())
														filename.pop_back();
													else
														continue;
												}
												else if((ret >= '0' && ret <= '9') || (ret >= 'a' && ret <= 'z') || (ret >= 'A' && ret <= 'Z'))
													filename.push_back(ret);
												else
													continue;

												fnv->draw(filename);
												ret = 0;

											} // Inner if
											else if(inputdevice->isLeftMouseButtonClicked())
											{
												SHORT ret = inputdevice->getItemClicked();

												switch(ret)
												{
													// The "Save" button was clicked.
													case 100	:	if(filename.empty())
																		continue;
																	else if(gs->filesystem->fileIsEqual(filename))
																		continue;
																	else
																	{
																		filename = "maps/" + filename + ".tdm";
																		gs->map->saveMap(filename);
																		gs->views.pop_back();
																		gs->console->clearConsole();
																		gs->status_view->addMessage(filename + " map saved");
																		inputdevice->reset();
																		gs->filesystem->readDirectory();
																		draw_all_views();
																		m_change = true;
																	}
																	fnv.release();
																	return;

													// The "Cancel button was clicked.
													case 101	:	gs->views.pop_back();
																	fnv.release();
																	return;

													default		:	continue;

												}
											}
											else if(inputdevice->isMouseHoveringOnItem())
											{

											}
										} // Inner while
									}
									return;

				// Tab
				case 9		:	{
									gs->tile_info_view->selectionInc();
									tile_selector( char(9) );
									m_change = true;
									std::auto_ptr<TileInfoView> tiv = std::auto_ptr<TileInfoView>(new TileInfoView(gs));
									gs->views.drawSingleView(tiv.get());
									std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(new StatusView(gs));
									gs->views.drawSingleView(sv.get());
								}
								return;

				// Carriage Return
				case 13		:	{
									tile_selector( char(13) );
									m_change = true;
									std::auto_ptr<TileInfoView> tiv = std::auto_ptr<TileInfoView>(new TileInfoView(gs));
									gs->views.drawSingleView(tiv.get());
								}
								return;

				// Escape
				case 27		:	exit(1);
				
				default		:	continue;
			} // switch

		} // If

		else if(inputdevice->isLeftMouseButtonClicked())
		{
			switch(inputdevice->getItemClicked())
			{
				// The string id associated with tab key was clicked ("Cycle Between Tiles").
				case 1		:	{
									gs->tile_info_view->selectionInc();
									tile_selector( char(9) );
									m_change = true;
									std::auto_ptr<TileInfoView> tiv = std::auto_ptr<TileInfoView>(new TileInfoView(gs));
									gs->views.drawSingleView(tiv.get());
								}
								return;

				// The string id associated with <enter> key was clicked ("Change Tile").
				case 2		:	{
									tile_selector( char(13) );
									m_change = true;
									std::auto_ptr<TileInfoView> tiv = std::auto_ptr<TileInfoView>(new TileInfoView(gs));
									gs->views.drawSingleView(tiv.get());
								}
								return;

				// The string id associated with 's' key was clicked ("Save (S)");
				case 3		:		{
										std::auto_ptr<FilenameView> fnv = std::auto_ptr<FilenameView>(new FilenameView(gs, SAVE_MAP));
										gs->views.push_back(fnv.get());
										m_change = true;

										std::string filename;

											while(true)
											{
												if(inputdevice->inputDeviceIsValid())
													inputdevice->poll();

												if(inputdevice->isKeyPressed())
												{
													char ret = inputdevice->getKeyPressed();

													if(filename.length() > 6) // Maximum of six characters for the length of the map file name.
													{
														if(ret == 13) // Carriage return
															return;
														else if(ret == 8) // Backspace
														{
															filename.pop_back();
															fnv->draw(filename);
															continue;
														}
														else
															continue;
													}

													if(ret == 13) // Carriage return
													{
														if(filename.empty())
															continue;
														else if(gs->filesystem->fileIsEqual(filename + ".tdm"))
															continue;
														else
														{
																filename = "maps/" + filename + ".tdm";
																gs->map->saveMap(filename);
																gs->views.pop_back();
																gs->console->clearConsole();
																gs->status_view->addMessage(filename + " map saved");
																inputdevice->reset();
																gs->filesystem->readDirectory();
																draw_all_views();
																m_change = true;
																fnv.release();
																return;
														}
													}
													else if(ret == 27) // Escape
													{
														filename.clear();
														break;
													}
													else if(ret == 8) // Backspace
													{
														if(!filename.empty())
															filename.pop_back();
														else
															continue;
													}
													else if((ret >= '0' && ret <= '9') || (ret >= 'a' && ret <= 'z') || (ret >= 'A' && ret <= 'Z'))
														filename.push_back(ret);
													else
														continue;

													fnv->draw(filename);
													ret = 0;

												} // Inner if
												else if(inputdevice->isLeftMouseButtonClicked())
												{
													SHORT ret = inputdevice->getItemClicked();

													// Locate which item in the FilenameView has been selected.
													//filename = fnv->getFileNameSelected();

													switch(ret)
													{
														// The "Save" button was clicked.
														case 100	:	if(filename.empty())
																			continue;
																		else if(gs->filesystem->fileIsEqual(filename + ".tdm"))
																			continue;
																		else
																		{
																			filename = "maps/" + filename + ".tdm";
																			gs->map->saveMap(filename);
																			gs->views.pop_back();
																			gs->console->clearConsole();
																			gs->status_view->addMessage(filename + " map saved");
																			inputdevice->reset();
																			gs->filesystem->readDirectory();
																			draw_all_views();
																			m_change = true;
																		}
																		fnv.release();
																		return;

														// The "Cancel button was clicked.
														case 101	:	gs->views.pop_back();
																		fnv.release();
																		return;

														default		:	continue;

													}
												}
												else if(inputdevice->isMouseHoveringOnItem())
												{

												}
											} // Inner while
										}
									return;

				// The string id associated with 'l' key was clicked ("Load (L)");
				// This means load the FilenameView view.
				case 4		:	{
										std::auto_ptr<FilenameView> fnv = std::auto_ptr<FilenameView>(new FilenameView(gs, LOAD_MAP));
										gs->views.push_back(fnv.get());
										m_change = true;
									
										std::string filename;

										while(true)
										{
											if(inputdevice->inputDeviceIsValid())
												inputdevice->poll();

											if(inputdevice->isKeyPressed())
											{
												char ret = inputdevice->getKeyPressed();

												if(filename.length() > 6)
												{
													if(ret == 13) // Carriage return
														return;
													else if(ret == 8) // Backspace
													{
														filename.pop_back();
														fnv->draw(filename);
														continue;
													}
													else
														continue;
												}

												if(ret == 13) // Carriage return
												{
													if(filename.empty())
														continue;
													else
													{
														// Do some file name sanity checks.
													}
												}
												else if(ret == 27) // Escape
												{
													filename.clear();
													break;
												}
												else if(ret == 8)
												{
													if(!filename.empty())
														filename.pop_back();
													else
														continue;
												}
												else if((ret >= '0' && ret <= '9') || (ret >= 'a' && ret <= 'z') || (ret >= 'A' && ret <= 'Z'))
													filename.push_back(ret);
												else
													continue;

												fnv->draw(filename);
												ret = 0;
											}
											else if(inputdevice->isLeftMouseButtonClicked())
											{
												SHORT ret = inputdevice->getItemClicked();

												// Locate which file name in the FilenameView has been selected.
												filename = fnv->getFileNameSelected();

												switch(ret)
												{
													// The "Load" button was clicked.
													case 99		:	if(filename.empty())
																		continue;
																	else
																	{
																		filename = "maps/" + filename;
																		gs->switchMap(filename);
																		gs->views.pop_back();
																		gs->console->clearConsole();
																		gs->status_view->addMessage(filename + " map loaded");
																		draw_all_views();
																		inputdevice->reset();
																		m_change = true;
																	}
																	fnv.release();
																	return;

													// The "Cancel button was clicked.
													case 101	:	gs->views.pop_back();
																	fnv.release();
																	return;

													default		:	continue;

												}
											}
											else if(inputdevice->isMouseHoveringOnItem())
											{
												inputdevice->setDialogBoxState();
												
												//switch(inputdevice->getItemHovered())
												//{
												//	default		:	inputdevice->unsetDialogBoxState();
																	continue;
												//}
											}
										} // Inner while
										
										return;
								}

				// The string associated with Escape key was clicked ("Go Back to Previous Menu ( ESC )").
				case 5		:	exit(1);
				
				default		:	continue;


			} // switch
		} // else if

		else if(inputdevice->isMouseHoveringOnItem())
		{
			// Do nothing on mouse hover event, let the inputdevice object handle it.
			//switch(inputdevice->getItemHovered())
			//{
			//	default		:	break;
			//}
		}

	} // Outer while

}

void Game::map_editor_init() {
	gs->gs = map_editor;
	gs->switchMap( 40, 20 );

	StatusView *sv = new StatusView(gs);
	gs->status_view = sv;
	gs->views.push_back(sv);

	InfoPaneView *ipv = new InfoPaneView( gs );
	gs->info_pane_view = ipv;
	gs->views.push_back( ipv );

	BoundingBox *bb = new BoundingBox( gs->console );
	gs->views.push_back( bb );

	MapView *mapview = new MapView( gs );
	gs->map_view = mapview;
	gs->views.push_back( mapview );

	TileInfoView *tiv = new TileInfoView( gs );
	gs->tile_info_view = tiv;
	gs->views.push_back( tiv );

	gs->map->update(reinterpret_cast<Model *>(gs->cursor));
}

void Game::play_init() {

	// This will ensure that StatusView will always be in the ViewManager
	// and will always be its first element. This is so we can retain the
	// StatusView's messages after the previous wave.
	StatusView *sv = new StatusView(gs);
	if(!gs->views.match(sv)) // Match not found, add StatusView.
	{
		gs->status_view = sv;
		gs->views.push_back(sv);
	}
	else // StatusView already exist in ViewManager, just delete this one.
		delete sv;

	InfoPaneView *ipv = new InfoPaneView( gs );
	gs->info_pane_view = ipv;
	gs->views.push_back( ipv );
	
	BoundingBox *bb = new BoundingBox( gs->console );
	gs->views.push_back( bb );

	MapView *mapview = new MapView( gs );
	gs->views.push_back( mapview );

	PlayerInfoView *piv = new PlayerInfoView( gs );
	gs->views.push_back( piv );

	TowerInfoView *tiv = new TowerInfoView( gs );
	gs->tower_info_view = tiv;
	gs->views.push_back( tiv );

	CreatureInfoView *civ = new CreatureInfoView( gs );
	gs->views.push_back( civ );

	gs->gs = edit_mode;
}

void Game::edit_mode_run() {
	
	if(!run_once)
	{
		run_once = true;
		draw_all_views();
	};

	inputdevice->validateMouseInput();

	while(true)
	{
		if(inputdevice->inputDeviceIsValid())
			inputdevice->poll();

		if(inputdevice->isKeyPressed())
		{
			switch(inputdevice->getKeyPressed())
			{
				case 's'	:	run_mode_init();
								m_change = true;
								return;

				case 'b'	:	if(gs->player->get_money() != 0)
								{
									show_chosen_tower();
									switch_to_build();
								}
								else
								{
									gs->status_view->addMessage("Insufficient Funds");
									m_change = true;
                  std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(new StatusView(gs));
                  gs->views.drawSingleView(sv.get());   //Display message in status view
								}
								return;

				case 27		:	std::exit(1);

				default		:	continue;
			} // switch
		} // if

		else if(inputdevice->isLeftMouseButtonClicked())
		{
			switch(inputdevice->getItemClicked())
			{
				case 1		:	if(gs->player->get_money() != 0)
								{
									show_chosen_tower();
									switch_to_build();
								}
								else
								{
									gs->status_view->addMessage("Insufficient Funds");
									m_change = true;
								}
								return;

				case 2		:	run_mode_init();
								m_change = true;
								return;

				case 3		:	std::exit(1);

				default		:	continue;
			} // switch
		} // else if

		else if(inputdevice->isMouseHoveringOnItem())
		{
			//switch(inputdevice->getItemHovered())
			//{
			//	default		:	continue;;
			//} // switch
		} // else if
	} // while

}

void Game::switch_to_build() {
	gs->gs = build_mode;
	gs->map->update(reinterpret_cast<Model *>(gs->cursor));

	InfoPaneView * ipv = new InfoPaneView(gs);
	gs->views.drawSingleView(ipv);
	delete ipv;
	
	setCursorImage();
}

void Game::switch_to_edit() {
	gs->gs = edit_mode;
}

void Game::build_mode_run() {
	inputdevice->validateMouseInput();

  draw_all_views();

  std::auto_ptr<MapView> mv = std::auto_ptr<MapView>(new MapView(gs));
  gs->views.drawSingleView(mv.get());   //Display the cursor when entering build mode


  std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(new StatusView(gs));
  gs->views.drawSingleView(sv.get());   //Display the selected tower stats when entering build mode

	while(true)
	{
		if(inputdevice->inputDeviceIsValid())
			inputdevice->poll();

		WORD ret = 0;
		Coord xy;
		Coord oldxy;

		if(inputdevice->isKeyPressed()) 
		{
			ret = inputdevice->getKeyPressed();
			xy = gs->cursor->get_coord();
			oldxy = xy;
		}
		else if(inputdevice->isLeftMouseButtonClicked())
		{
			ret = inputdevice->getItemClicked();
		}
		else if(inputdevice->isMouseHoveringOnItem())
		{
			//ret = inputdevice->getItemHovered();
		}

		switch(ret)
		{
			case 37:         //left arrow
				if( xy.x > 0 ) {
					xy.x = xy.x - 1;
					update_cursor( xy, oldxy );
				}
				return;
			case 39:         //right arrow
				if( xy.x < gs->map->getWidth() - 1 ) {
					xy.x = xy.x + 1;
					update_cursor( xy, oldxy );
				}
				return;
			case 38:         //up arrow
				if( xy.y > 0 ) {
					xy.y = xy.y - 1;
					update_cursor( xy, oldxy );
				}
				return;
			case 40:         //down arrow
				if( xy.y < gs->map->getHeight() - 1 ) {
					xy.y = xy.y + 1;
					update_cursor( xy, oldxy );
				}
				return;
			case 9:	// tab
				gs->tower_info_view->selectionInc();
				setCursorImage();
				show_chosen_tower();
				m_change = true;
				return;
			case 13:	// Carriage return
				if (gs->map->check_can_build(const_cast<Coord &>(gs->cursor->get_coord()))) {   //checks if player can build on top of
					Coord tower_xy = gs->cursor->get_coord();
					string name;
				
					switch( gs->tower_info_view->getSelection() ) {
						case 0:
							t = new Cannon(gs->cursor->get_coord().x, gs->cursor->get_coord().y);
							name = "Cannon";
							break;
						case 1:
							t = new Sniper(gs->cursor->get_coord().x, gs->cursor->get_coord().y);
							name = "Sniper";
							break;
						case 2:
							t = new Laser(gs->cursor->get_coord().x, gs->cursor->get_coord().y);
							name = "Laser";
							break;
					}
					WORD tile_color = gs->map->get_base_color( tower_xy );
					t->set_color( tile_color );

					if( gs->player->get_money() >= t->get_cost() ) {
						gs->player->set_money(gs->player->get_money() - t->get_cost());   //update player money
						tower_xy.layer = 1;                                               //makes sure that tower is displayed on proper layer
						t->set_coord(tower_xy);
						gs->map->update(t);                                               //update with tower
						gs->status_view->addMessage( name + " tower built" );
						gs->towerMgr()->add_tower(t);
					}
					else {
						gs->status_view->addMessage( "Insufficient Funds" );
						delete t;
					}
					m_change = true;
				}
				else {
					m_change = true;
					gs->status_view->addMessage( "Cannot build here" );
				}
				return;
			case 27:                                        //if player hits ESC, cancel build
				gs->map->clear_cursor(gs->cursor->get_coord());     //clear cursor at current location
				m_change = true;
				switch_to_edit();
				gs->console->clearConsole();
				run_once = false;
				inputdevice->reset();
				return;
			case 46:	// Delete key
				Coord temp = gs->cursor->get_coord();
				if (gs->map->sell_tower(temp))		//sell_tower will return true if the tower exists at temp
					gs->towerMgr()->remove_tower_at(temp);
				else
					gs->status_view->addMessage("No tower here");
				m_change = true;
				return;
		}  //end switch
  } //end while
}

void Game::run_mode_init() {

  // Setup wave.
  gs->creatureMgr()->setup();

  gs->info_pane_view->addMessage("Wave...");
  gs->info_pane_view->addMessage("");
  gs->info_pane_view->addMessage("");
  gs->info_pane_view->addMessage("");

  gs->gs = run_mode;
  run_once = false;

  InfoPaneView * ipv = new InfoPaneView(gs);
  gs->views.drawSingleView(ipv);
  delete ipv;
}

void Game::run_mode_run(uint32 delta_time)
{
	if (delta_time >= 100)
	{
		// Advance wave by one tile.
		gs->creatureMgr()->advance();
		gs->towerMgr()->range_check();
	}

	//**** An attempt to reduce the overall flickering ****//

	if(!run_once)
	{
		run_once = true;

		std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(new StatusView(gs));
		gs->views.drawSingleView(sv.get());

		std::auto_ptr<CreatureInfoView> civ = std::auto_ptr<CreatureInfoView>(new CreatureInfoView(gs));
		gs->views.drawSingleView(civ.get());

		std::auto_ptr<BoundingBox> bb(new BoundingBox(gs->console));
		gs->views.erase(bb.get());
		bb.release();

	}

	if(gs->player->get_lives() < lives_count)
	{
		lives_count--;

		std::auto_ptr<PlayerInfoView> piv = std::auto_ptr<PlayerInfoView>(new PlayerInfoView(gs));
		gs->views.drawSingleView(piv.get());
	}

	std::auto_ptr<MapView> mv = std::auto_ptr<MapView>(new MapView(gs));
	gs->views.drawSingleView(mv.get());

	//*****************************************//

	if(gs->creatureMgr()->is_wave_complete())
	{
		// Go to the next level...
		if(gs->player->get_lives() > 0)
		{
			std::string str("Level ");
			str += (char) (((int)'0') + gs->creatureMgr()->get_current_level());
			str += " done...";
			gs->status_view->addMessage(str);

			std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(new StatusView(gs));
			gs->views.drawSingleView(sv.get());

			std::auto_ptr<CreatureInfoView> civ = std::auto_ptr<CreatureInfoView>(new CreatureInfoView(gs));
			gs->views.drawSingleView(civ.get());

			exit_run_mode();
		}
		else // Game over
		{
			gs->status_view->addMessage("Game Over!");
			std::auto_ptr<StatusView> sv = std::auto_ptr<StatusView>(new StatusView(gs));
			gs->views.drawSingleView(sv.get());

			WORD status = 1, ret = 0;
			GameOverView * gov = new GameOverView(gs);
			gov->setStatus(status);
			gs->views.push_back(gov);
			gs->views.drawSingleView(gov);
			
			gs->gs = game_over;

			m_change = true;

			while(true)
			{
				if(inputdevice->inputDeviceIsValid())
					inputdevice->poll();

				if(inputdevice->isKeyPressed()) 
					ret = inputdevice->getKeyPressed();
				else if(inputdevice->isLeftMouseButtonClicked())
					ret = inputdevice->getItemClicked();

				switch(ret)
				{
					case 0		:	break;
					case 'c'	:	
					case 13		:	{
									while(gs->views.size() > 0)
										gs->views.pop_back();

										delete gs;
										gs = new GameState(this);

										main_menu_init();
										gs->console->clearConsole();
										run_once = false;
										lives_count = gs->player->get_lives();

										// This needs to be called last or mouse input will
										// not respond to events in main menu.
										delete inputdevice;
										inputdevice = new InputDeviceHandler(gs);
										
										return;
								  }
					default		:	break;
				}
			}
		}
	}
}

void Game::exit_run_mode() {

	gs->views.clear();

	play_init();
	gs->console->clearConsole();
	m_change = true;
	
	inputdevice->reset();
	run_once = false;
}

void Game::draw_all_views() {
	if( m_change ) {
		gs->views.drawViews();
		m_change = false;
	}
}

void Game::update_cursor( Coord newxy, Coord oldxy ) {
	gs->cursor->set_coord( newxy.x, newxy.y );
	gs->map->clear_cursor(oldxy);
	gs->map->update(reinterpret_cast<Model *>(gs->cursor));
}

void Game::setCursorImage() {
	switch( gs->tower_info_view->getSelection() ) {
		case 0:
			gs->cursor->set_img( char( 227 ) );
			break;
		case 1:
			gs->cursor->set_img( '+' );
			break;
		case 2:
			gs->cursor->set_img( char( 248 ) );
			break;
	}
}

void Game::show_chosen_tower() {
  switch (gs->tower_info_view->getSelection()) {
    case 0:
      gs->status_view->addMessage("Canon Selected");
	  gs->status_view->addMessage("     Radius-1, Damage-1, Speed-1, Type-BLUDGEON");
      break;
    case 1:
      gs->status_view->addMessage("Sniper Selected");
	  gs->status_view->addMessage("     Radius-5, Damage-2, Speed-3, Type-PIERCE");
      break;
    case 2:
      gs->status_view->addMessage("Laser Selected");
	  gs->status_view->addMessage("     Radius-3, Damage-4, Speed-2, Type-ELEMENTAL");
      break;
  }
}

void Game::tile_selector( char input ) {
	Coord cursor = gs->cursor->get_coord();
	string message;

	switch( gs->tile_info_view->getSelection() ){
		case 0:
			if( input == char(9) )
				message = "Path Selected";
			else if( input == char(13) ) {
				Tile *new_tile = new Tile( cursor.x, cursor.y, ' ', false, true, 0x70);
				gs->map->newTile( new_tile );
			}
			break;
		case 1:
			if( input == char(9) )
				message = "Grass Selected";
			else if( input == char(13) ) {
				Tile *new_tile = new Tile( cursor.x, cursor.y, ' ', true, false, 0x20);
				gs->map->newTile( new_tile );
			}
			break;
		case 2:
			if( input == char(9) )
				message = "Rocks Selected";
			else if( input == char(13) ) {
				Tile *new_tile = new Tile( cursor.x, cursor.y, (char) 178, false, false, 0x28);
				gs->map->newTile( new_tile );
			}
			break;
		case 3:
			if( input == char(9) )
				message = "Base Selected";
			else if( input == char(13) ) {
				if( cursor.x == 0 || cursor.y == 0 || cursor.y == gs->map->getHeight() - 1 || cursor.x == gs->map->getWidth() - 1 ) {
					// remove old start position
					Coord old_end = gs->map->get_end();
					if( old_end.x != - 1 ) {
						Tile *new_tile = new Tile( old_end.x, old_end.y, ' ', true, false, 0x20);
						gs->map->newTile( new_tile );
					}

					// set new start position
					Tile *new_tile = new Tile( cursor.x, cursor.y, char(127), false, true, 0x70);
					gs->map->set_end( new_tile->get_coord() );
					gs->map->newTile( new_tile );
					message = "Base tile placed";
				}
				else
					message = "Can not place a base tile here";
			}
			break;
		case 4:
			if( input == char(9) )
				message = "Start Selected";
			else if( input == char(13) ) {
				if( cursor.x == 0 || cursor.y == 0 || cursor.y == gs->map->getHeight() - 1 || cursor.x == gs->map->getWidth() - 1 ) {
					// remove old start position
					Coord old_start = gs->map->get_start();
					if( old_start.x != - 1 ) {
						Tile *new_tile = new Tile( old_start.x, old_start.y, ' ', true, false, 0x20);
						gs->map->newTile( new_tile );
					}

					// set new start position
					Tile *new_tile = new Tile( cursor.x, cursor.y, 'S', false, true, 0x70);
					gs->map->set_start( new_tile->get_coord() );
					gs->map->newTile( new_tile );
					message = "Start tile placed";
				}
				else
					message = "Can not place a start tile here";
			}
			break;
	}

	gs->status_view->addMessage( message );
}

void Game::loadFileDialogBox(SHORT s)
{
	std::auto_ptr<BoundingBox> bb (new BoundingBox( gs->console ));
	bb->draw();

	std::auto_ptr<FilenameView> fnv = std::auto_ptr<FilenameView>(new FilenameView(gs, s));
	gs->views.push_back(fnv.get());
	m_change = true;

	std::string filename;

	while(true)
	{
		if(inputdevice->inputDeviceIsValid())
			inputdevice->poll();

		if(inputdevice->isKeyPressed())
		{
			char ret = inputdevice->getKeyPressed();

			if(filename.length() > 6)
			{
				if(ret == 13) // Carriage return
					return;
				else if(ret == 8) // Backspace
				{
					filename.pop_back();
					fnv->draw(filename);
					continue;
				}
				else
					continue;
			}

			if(ret == 13) // Carriage return
			{
				// Locate which file name in the FilenameView has been selected.
				if(!(gs->filesystem->fileIsEqual(filename+".tdm") == true))
					continue;
				else if(filename.empty())
					continue;
				else
				{
					filename = "maps/" + filename + ".tdm";
					gs->switchMap(filename);
					gs->views.pop_back();
					gs->console->clearConsole();
					gs->status_view->addMessage(filename + " map loaded");
					draw_all_views();
					inputdevice->reset();
					m_change = true;
				}
				fnv.release();
				return;
			}
			else if(ret == 27) // Escape
			{
				filename.clear();
				break;
			}
			else if(ret == 8)
			{
				if(!filename.empty())
					filename.pop_back();
				else
					continue;
			}
			else if((ret >= '0' && ret <= '9') || (ret >= 'a' && ret <= 'z') || (ret >= 'A' && ret <= 'Z'))
				filename.push_back(ret);
			else
				continue;

			fnv->draw(filename);
			ret = 0;
		}
		else if(inputdevice->isLeftMouseButtonClicked())
		{
			inputdevice->setDialogBoxState();
			SHORT ret = inputdevice->getItemClicked();

			// Locate which file name in the FilenameView has been selected.
			filename = fnv->getFileNameSelected();

			switch(ret)
			{
				// The "Load" button was clicked.
				case 99		:	if(filename.empty())
									continue;
								else
								{
									filename = "maps/" + filename;
									gs->switchMap(filename);
									gs->views.pop_back();
									gs->console->clearConsole();
									gs->status_view->addMessage(filename + " map loaded");
									draw_all_views();
									inputdevice->reset();
									m_change = true;
								}
								fnv.release();
								return;

				// The "Cancel button was clicked.
				case 101	:	gs->views.pop_back();
								fnv.release();
								return;

				default		:	continue;

			}
		}
		else if(inputdevice->isMouseHoveringOnItem())
		{
			inputdevice->setDialogBoxState();
												
			//switch(inputdevice->getItemHovered())
			//{
			//	default		:	inputdevice->unsetDialogBoxState();
								continue;
			//}
		}
	} // Inner while
										
	//return;

	//bb.release();

}

void Game::loadAboutView()
{
	std::auto_ptr<BoundingBox> bb (new BoundingBox(gs->console));
	bb->draw();

	std::auto_ptr<AboutView> av (new AboutView(gs));
	av->draw();

	while(true)
	{
		if(inputdevice->inputDeviceIsValid())
			inputdevice->poll();

		if(inputdevice->isKeyPressed())
		{
			char ret = inputdevice->getKeyPressed();

			if(ret == 13 || ret == 'c')
				return;
		}
		else if(inputdevice->isLeftMouseButtonClicked())
		{
			switch(inputdevice->getItemClicked())
			{
				default		:	return;
			}
		}
	}
}

void Game::loadHelpView()
{
	std::auto_ptr<BoundingBox> bb (new BoundingBox(gs->console));
	bb->draw();

	std::auto_ptr<HelpView> hv (new HelpView(gs));
	hv->draw();

		while(true)
	{
		if(inputdevice->inputDeviceIsValid())
			inputdevice->poll();

		if(inputdevice->isKeyPressed())
		{
			char ret = inputdevice->getKeyPressed();

			if(ret == 13 || ret == 'c')
				return;
		}
		else if(inputdevice->isLeftMouseButtonClicked())
		{
			switch(inputdevice->getItemClicked())
			{
				default		:	gs->console->clearConsole();
								ShellExecuteA(NULL, "open", "http://code.google.com/p/t-defense-project/wiki/ReadmeRunTowerDefense",
												NULL, NULL, SW_SHOWNORMAL);
								return;
			}
		}
	}
}