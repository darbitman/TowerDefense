#include "map.h"
#include "tower.h"
#include <fstream>
#include <time.h>

// blank map initializer
Map::Map( int width = 15, int height = 15 ){
	stage.resize(3);      //create 3 layers, 0 = map, 1 = actor/creature, 2 = cursor
	this->width = width;
	this->height = height;
	for (int i = 0; i < 3; i++) {
		stage[i].resize(width);
		for (int j = 0; j < width; j++)
		  stage[i][j].resize(height);
	}

	for (int i = 0; i < height; i++) {     //can    is
		for (int j = 0; j < width; j++) {//build  path                        
          stage[0][j][i] = new Tile(j, i, ' ', true, false, 0x20);    //black on green
      }
    }
	start.x = -1;
	start.y = -1;
	end.x = -1;
	end.y = -1;
}

Map::Map( string mapname ) {
	char base = static_cast<char>(127);   //home base
	this->map_name = mapname;

	stage.resize(3);      //create 3 layers, 0 = map, 1 = actor/creature, 2 = cursor
  ifstream map( mapname, ios::in); //"maps/basic5.tdm"
  map >> width;
  map >> height;
  char map_img;
  map.get(map_img); //ignore the newline character
	for (uint32 i = 0; i < 3; i++) {
		 stage[i].resize(width);
    for (uint32 j = 0; j < width; j++)
      stage[i][j].resize(height);
  }

  for (uint32 i = 0; i < height; i++) {
    for (uint32 j = 0; j < width+1; j++) {
      map.get(map_img);
      switch (map_img) {                     //can    is
        case ('*') :                         //build  path
          stage[0][j][i] = new Tile(j, i, ' ', true, false, 0x20);    //black on green
          break;
        case ('P') :
          stage[0][j][i] = new Tile(j, i, ' ', false, true, 0x70);    //0x70 = black on gray; creates a gray background
          break;
		case ('X') :
          stage[0][j][i] = new Tile(j, i, char(178), false, false, 0x28);    // rocks tiles, not buildable
          break;
        case ('S') :
          stage[0][j][i] = new Tile(j, i, ' ', false, true, 0x70);
          start.layer = 1;
          start.x = j;
          start.y = i;
          break;
        case ('E') :
          stage[0][j][i] = new Tile(j, i, base, false, true, 0x70);
          end.layer = 1;
          end.x = j;
          end.y = i;
          break;
        default:
          break;
      }


    }
  }
  map.close();
}

Model* Map::get_model( Coord xy ) {                               //return model that's on the top most layer
	if (stage[2][xy.x][xy.y] != NULL) return stage[2][xy.x][xy.y];
	else if (stage[1][xy.x][xy.y] != NULL) return stage[1][xy.x][xy.y];
	else /*(stage[0][xy.x][xy.y] != NULL)*/ return stage[0][xy.x][xy.y];
}

WORD Map::get_base_color( Coord xy ) {
	return stage[0][xy.x][xy.y]->get_color();
}

Model* Map::get_creature(const Coord& xy)
{
  return stage[1][xy.x][xy.y] != NULL ? stage[1][xy.x][xy.y] : NULL;
}

void Map::update( Model* update_model) {                    //updates map with whatever model is created
	Coord newxy = update_model->get_coord();
  stage[newxy.layer][newxy.x][newxy.y] = update_model;
}

void Map::update( Model* update_model, Coord current_creature) {     //when creature moves, previous tile must be invalidated
  stage[1][current_creature.x][current_creature.y] = NULL;
	Coord newxy = update_model->get_coord();
  stage[newxy.layer][newxy.x][newxy.y] = update_model;
}

void Map::clear_cursor(Coord oldxy) {
  stage[2][oldxy.x][oldxy.y] = NULL;
}

void Map::clear_creature(Coord oldxy) {
  stage[1][oldxy.x][oldxy.y] = NULL;
}

bool Map::check_can_build(Coord &current) const {
  if (stage[1][current.x][current.y] == NULL &&
    reinterpret_cast<Tile*>(stage[0][current.x][current.y])->can_build())
    return true;
  return false;
}

Map::~Map(void) {
  for (int k = 0; k < 3; k++)
	  for (int i = 0; i < 15; i++)
		  for (int j = 0; j < 15; j++) 
			  delete stage[k][i][j];
}

const Coord& Map::get_start() {return start;}
void Map::set_start( Coord xy ) {
	start.x = xy.x;
	start.y = xy.y;
}

const Coord& Map::get_end() {return end;}
void Map::set_end( Coord xy ) {
	end.x = xy.x;
	end.y = xy.y;
}

Coord Map::get_next_path(const Creature* creature)
{
  Coord pos = creature->get_coord();
  Coord prev = creature->get_previous_coord();
  Coord c = pos;
  Coord seed;        //where to start searching for divergent path
  uint32 direction = 0;          //stores direction that is possible for creature to move towards
  uint32 num_directions = 0;
  // Scan right, up, down, or left for an available path.
  if (reinterpret_cast<Tile*>(stage[0][pos.x+1][pos.y])->is_path() &&           //right
      stage[1][pos.x+1][pos.y] == NULL    &&
      pos.x+1 != prev.x &&
      (pos.x+1 != creature->converge.x ||
      pos.y != creature->converge.y))
  {
    direction |= RIGHT;
    num_directions++;
  }
  if (pos.y-1 > 0 &&
          reinterpret_cast<Tile*>(stage[0][pos.x][pos.y-1])->is_path() &&       //up
          stage[1][pos.x][pos.y-1] == NULL    &&
          pos.y-1 != prev.y &&
          (pos.y-1 != creature->converge.y ||
          pos.x != creature->converge.x))
  {
    direction |= UP;
    num_directions++;
  }
  if (reinterpret_cast<Tile*>(stage[0][pos.x][pos.y+1])->is_path() &&      //down
          stage[1][pos.x][pos.y+1] == NULL     &&
          pos.y+1 != prev.y &&
          (pos.y+1 != creature->converge.y ||
          pos.x != creature->converge.x))
  {
    direction |= DOWN;
    num_directions++;
  }
  if (pos.x-1 > 0 &&
          reinterpret_cast<Tile*>(stage[0][pos.x-1][pos.y])->is_path() &&      //left
          stage[1][pos.x-1][pos.y] == NULL    &&
          pos.x-1 != prev.x &&
          (pos.x-1 != creature->converge.x ||
          pos.y != creature->converge.y))
  {
    direction |= LEFT;
    num_directions++;
  }

//checks the directions that a creature can travel
//depending on the random number, the scan order for directions changes
//masks the direction bits to check whether that direction was possible
//if a path is possible, then move in that direction
  if (num_directions > 1) {
    srand(int(time(NULL)));
    while (1) {
      uint32 random = rand() % 4;
      if (random == 0) {
        if ((direction & RIGHT) == RIGHT) {
          c.x = pos.x + 1;
          direction &= ~RIGHT;        //save the other direction that was possible
          break;
        }
        else if ((direction & DOWN) == DOWN) {
          c.y = pos.y + 1;
          direction &= ~DOWN;        //save the other direction that was possible
          break;
        }
        else if ((direction & LEFT) == LEFT) {
          c.x = pos.x - 1;
          direction &= ~LEFT;        //save the other direction that was possible
          break;
        }
        else if ((direction & UP) == UP) {
          c.y = pos.y - 1;
          direction &= ~UP;        //save the other direction that was possible
          break;
        }
      }

      if (random == 1) {
        if ((direction & DOWN) == DOWN) {
          c.y = pos.y + 1;
          direction &= ~DOWN;        //save the other direction that was possible
          break;
        }
        else if ((direction & LEFT) == LEFT) {
          c.x = pos.x - 1;
          direction &= ~LEFT;        //save the other direction that was possible
          break;
        }
        else if ((direction & UP) == UP) {
          c.y = pos.y - 1;
          direction &= ~UP;        //save the other direction that was possible
          break;
        }
        else if ((direction & RIGHT) == RIGHT) {
          c.x = pos.x + 1;
          direction &= ~RIGHT;        //save the other direction that was possible
          break;
        }
      }

      if (random == 2) {
        if ((direction & LEFT) == LEFT) {
          c.x = pos.x - 1;
          direction &= ~LEFT;        //save the other direction that was possible
          break;
        }
        else if ((direction & UP) == UP) {
          c.y = pos.y - 1;
          direction &= ~UP;        //save the other direction that was possible
          break;
        }
        else if ((direction & RIGHT) == RIGHT) {
          c.x = pos.x + 1;
          direction &= ~RIGHT;        //save the other direction that was possible
          break;
        }
        else if ((direction & DOWN) == DOWN) {
          c.y = pos.y + 1;
          direction &= ~DOWN;        //save the other direction that was possible
          break;
        }
      }

      if (random == 3) {
        if ((direction & UP) == UP) {
          c.y = pos.y - 1;
          direction &= ~UP;        //save the other direction that was possible
          break;
        }
        else if ((direction & RIGHT) == RIGHT) {
          c.x = pos.x + 1;
          direction &= ~RIGHT;        //save the other direction that was possible
          break;
        }
        else if ((direction & DOWN) == DOWN) {
          c.y = pos.y + 1;
          direction &= ~DOWN;        //save the other direction that was possible
          break;
        }
        else if ((direction & LEFT) == LEFT) {
          c.x = pos.x - 1;
          direction &= ~LEFT;        //save the other direction that was possible
          break;
        }
      }
    }  //END while (1)

    /*  GENERATE SEED  */   //This is the location where the scan function will start
    if ((direction & LEFT) == LEFT) {
      seed.x = pos.x - 1;
      seed.y = pos.y;
    }
    else if ((direction & UP) == UP) {
      seed.x = pos.x;
      seed.y = pos.y - 1;
    }
    else if ((direction & RIGHT) == RIGHT) {
      seed.x = pos.x + 1;
      seed.y = pos.y;
    }
    else if ((direction & DOWN) == DOWN) {
      seed.x = pos.x;
      seed.y = pos.y + 1;
    }

    const_cast<Creature *>(creature)->converge = scan_forward(seed, pos);
  } //END if (num_directions > 1)
  else if (num_directions == 1) {
    switch (direction) {
      case RIGHT:
        c.x = pos.x + 1;
        break;
      case UP:
        c.y = pos.y - 1;
        break;
      case DOWN:
        c.y = pos.y + 1;
        break;
      case LEFT:
        c.x = pos.x - 1;
        break;
    }
  }
  else {    // if nowhere to move, then remain at current location
    const_cast<Creature *>(creature)->set_previous_coord(prev);
    return pos;
  }

  const_cast<Creature *>(creature)->set_previous_coord(pos);
  return c;
}

int Map::getHeight(){
	return height;
}

int Map::getWidth(){
	return width;
}

void Map::newTile( Model *new_tile ) {
	Coord xy = new_tile->get_coord();
	delete stage[0][xy.x][xy.y];
	stage[0][xy.x][xy.y] = new_tile;
}

void Map::saveMap( string filename ) {
	
	ofstream map_file ( filename, ios::trunc );

	map_file << width << ' ' << height << '\n';

	for( uint32 j = 0; j < height; j++ ) {
		for( uint32 i = 0; i < width; i ++ ) {
			if( reinterpret_cast<Tile*>(stage[0][i][j])->can_build() )
				map_file << '*';
			else {
				if( reinterpret_cast<Tile*>(stage[0][i][j])->is_path() ) {
					if( start.x == i && start.y == j )
						map_file << 'S';
					else if( end.x == i && end.y == j )
						map_file << 'E';
					else
						map_file << 'P';
				}
				else
					map_file << 'X';
			}
		}
		map_file << '\n';
	}

	map_file.close();
}

Coord Map::scan_forward(Coord seed, Coord prev) {
  uint32 num_directions = 0;
  uint32 direction = 0;
  while (1) {
    num_directions = 0;
    direction = 0;

    if (reinterpret_cast<Tile *>(stage[0][seed.x-1][seed.y])->is_path() && (seed.x-1 != prev.x)) {
      num_directions++;
      direction |= LEFT;
    }
    if (reinterpret_cast<Tile *>(stage[0][seed.x][seed.y-1])->is_path() && (seed.y-1 != prev.y)) {
      num_directions++;
      direction |= UP;
    }
    if (reinterpret_cast<Tile *>(stage[0][seed.x+1][seed.y])->is_path() && (seed.x+1 != prev.x)) {
      num_directions++;
      direction |= RIGHT;
    }
    if (reinterpret_cast<Tile *>(stage[0][seed.x][seed.y+1])->is_path() && (seed.y+1 != prev.y)) {
      num_directions++;
      direction |= DOWN;
    }

    if (num_directions > 1) return prev;    //if we're at an intersection (paths converge) return the coordinates of the tile just before the intersection

    prev = seed;
    switch (direction) {
      case LEFT:
        seed.x -= 1;
        break;
      case UP:
        seed.y -= 1;
        break;
      case RIGHT:
        seed.x += 1;
        break;
      case DOWN:
        seed.y += 1;
        break;
    }
  }  //END while (1)
}

bool Map::sell_tower(Coord &xy) const {
  if (stage[1][xy.x][xy.y] == NULL)
	 return false;
  else
	  return true;
}

string Map::getMapName() { 
	string name = map_name.substr( 5 );
	name = name.substr( 0, name.length() - 4 );
	return name; 
}