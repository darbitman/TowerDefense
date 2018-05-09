#ifndef Coord_h__
#define Coord_h__

class Coord
{
public:
  Coord();
  Coord(int x, int y);
  Coord(int x, int y, int layer);
  Coord(const Coord& other);

  bool operator==(const Coord &xy) const {
    return (this->x == xy.x && this->y == xy.y);
  }

  int x;
  int y;
  int layer;
};

#endif // Coord_h__
