#pragma once

#include "Coord.h"
#include "tile.h"
#include "creature.h"

class Map
{
public:
	Map( string );
	Map( int, int );
	~Map(void);

	Model* get_model(Coord);
	Model* get_creature(const Coord&);
	void update(Model *);
	void update(Model *, Coord);
	void newTile(Model *);
	void clear_cursor(Coord);
	void clear_creature(Coord);
	bool sell_tower(Coord &) const;
	bool check_can_build(Coord &) const;        //return true if can build at current location
	const Coord& get_start();
	void set_start(Coord);
	const Coord& get_end();
	void set_end(Coord);
	// Returns the next available path.
	Coord get_next_path(const Creature* creature);
	WORD get_base_color( Coord );
	int getWidth();
	int getHeight();
	string getMapName();

	void saveMap( string );
  Coord scan_forward(Coord seed, Coord prev);

private:
  Coord start;
  Coord end;
  uint32 width;
  uint32 height;
  std::vector< std::vector< std::vector< Model* > > > stage;
  enum {LEFT = 1, UP = 2, RIGHT = 4, DOWN = 8};
  string map_name;
};