#include "tile.h"

Tile::Tile(int x, int y, int8 img, bool build, bool path, WORD _color, int layer)
  : Model(img, Coord(x, y, layer), _color)
{
  m_build = build;
  m_path = path;
}

bool Tile::can_build() const
{
  return m_build;
}

bool Tile::is_path() const
{
  return m_path;
}