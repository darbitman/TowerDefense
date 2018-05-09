#include "model.h"

Model::Model(int8 img, Coord xy, WORD _color)
{
  color = _color;
  m_img = img;
  m_xy = xy;
}

const Coord& Model::get_coord() const
{
  return m_xy;
}

int8 Model::get_img() const
{
  return m_img;
}

void Model::set_coord(int32 x, int32 y)
{
  m_xy.x = x;
  m_xy.y = y;
}

void Model::set_coord(const Coord& xy)
{
  m_xy = xy;
}

void Model::set_img(int8 img)
{
  m_img = img;
}

WORD Model::get_color() const {
  return color;
}

void Model::set_color(WORD _color) {
  color = _color;
}