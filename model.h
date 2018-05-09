#pragma once
#include "Config.h"
#include "Coord.h"

enum DamageType
{
  BLUDGEON,
  PIERCE,
  SLASH,
  ELEMENTAL,
  NONE,
};

class Model
{
public:
  virtual ~Model() { }
  const Coord& get_coord() const;
  int8 get_img() const;

  void set_coord(int32, int32);
  void set_coord(const Coord&);
  void set_img(int8);
  WORD get_color() const;
  void set_color(WORD _color);

protected:
  explicit Model(int8 img, Coord xy, WORD _color);
  int8  m_img;
  Coord m_xy;
  WORD color;
};
