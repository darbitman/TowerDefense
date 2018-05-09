#pragma once
#include "Config.h"
#include "model.h"

class Tile :	public Model {
 public:
  Tile(int _x, int _y, int8 _img, bool can_build, bool is_path, WORD _color = 0x07, int layer = 0);

  bool can_build() const;     //returns true if player can build on top
  bool is_path() const;       //returns true if tile is a path object

private:
  bool m_build;     //true if player can build on top
  bool m_path;      //true if path tile
};