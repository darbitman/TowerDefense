#include "Coord.h"

Coord::Coord()
{
  x = 0;
  y = 0;
  layer = 0;
}

Coord::Coord(int x, int y)
{
  this->x = x;
  this->y = y;
  this->layer = 0;
}

Coord::Coord(int x, int y, int layer)
{
  this->x = x;
  this->y = y;
  this->layer = layer;
}

Coord::Coord(const Coord& other)
{
  this->x = other.x;
  this->y = other.y;
  this->layer = other.layer;
}