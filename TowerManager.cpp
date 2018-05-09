#include "TowerManager.h"

TowerManager::TowerManager(Map* map, Player* player, GameState *_gs)
{
  m_map = map;
  m_player = player;
  m_towers.clear();
  gs = _gs;
}

void TowerManager::add_tower(Tower* tower)
{
  m_towers.push_back(tower);
}

void TowerManager::remove_tower_at(const Coord& xy)
{
  list<Tower*>::iterator itr = m_towers.begin();
  for (; itr != m_towers.end(); ++itr)
  {
    if ((*itr)->get_coord() == xy)
    {
      Tower* t = (*itr);
      m_towers.erase(itr);
      m_map->clear_creature(t->get_coord());
      m_player->add_money(t->sell());
      delete t;
      break;
    }
  }
}

void TowerManager::reset()
{
  list<Tower*>::iterator itr = m_towers.begin();
  while (itr != m_towers.end())
  {
    Tower* tower = (*itr);
    m_towers.erase(itr++);
    delete tower;
  }
  m_towers.clear();
}

void TowerManager::range_check()
{
  list<Tower*>::iterator itr = m_towers.begin();
  Tower* tower;
  for (; itr != m_towers.end(); ++itr)
  {
    tower = (*itr);
    Coord xy = tower->get_coord();
    int32 modifier = 1;
    for (int32 i = 0; i < tower->get_radius(); ++i)
    {
      xy.x = xy.x + 1; // Check East
      if (_is_in_range(tower, xy))
      {
        break;
      }

      // Check Southeast
      for (int32 j = 0; j < modifier; ++j)
      {
        xy.y = xy.y + 1;
        if (_is_in_range(tower, xy))
        {
          break;
        }
      }

      modifier++;
      // Check South to Southwest
      for (int32 j = 0; j < modifier; ++j)
      {
        xy.x = xy.x - 1;
        if (_is_in_range(tower, xy))
        {
          break;
        }
      }

      // Check Southwest to Northwest
      for (int32 j = 0; j < modifier; ++j)
      {
        xy.y = xy.y - 1;
        if (_is_in_range(tower, xy))
        {
          break;
        }
      }

      // Check Northwest to Northeast
      for (int32 j = 0; j < modifier; ++j)
      {
        xy.x = xy.x + 1;
        if (_is_in_range(tower, xy))
        {
          break;
        }
      }

      modifier++;
    }
  }
}

bool TowerManager::_is_in_range(Tower* tower, const Coord& xy)
{
  if (xy.x < 0 || xy.y < 0 || xy.x >= m_map->getWidth() || xy.y >= m_map->getHeight())
  {
    return false;
  }

  Model* model = m_map->get_creature(xy);
  if (model != NULL)
  {
    if (Creature* creature = dynamic_cast<Creature*>(model))
    {
		WORD base_color = tower->get_color();
      tower->set_color( ( base_color & 0xF0 ) | 0x0E);   //leave bg, but set foreground to yellow
      tower->attack(creature);
      creature->set_color();
      gs->views.drawViews();
      tower->set_color( base_color );       //reset color when idle
      gs->views.drawViews();
      return true;
    }
  }

  return false;
}