#include "CreatureManager.h"

CreatureManager::CreatureManager(Map* _map, GameState* _gs, Player* player)
{
  m_map = _map;
  m_gs = _gs;
  m_player = player;
  current_level = 0;
  _populate_levels();
}

void CreatureManager::setup()
{
  m_wave.clear();
  vector<pair<uint32, CreatureType> > wave_level = m_levels[current_level++];
  for (uint32 i = 0; i < wave_level.size(); i++)
  {
    uint32 amount = wave_level[i].first;
    for (uint32 j = 0; j < amount; j++)
    {
      Creature* creature = new Creature(m_map->get_start().x, m_map->get_start().y, wave_level[i].second);
      m_wave.push_back(creature);
    }
  }
}

void CreatureManager::advance()
{
  list<Creature*>::iterator itr = m_wave.begin();
  Creature* creature;
  while (itr != m_wave.end())
  {
    creature = (*itr);
    if (creature->is_dead())
    {
      m_player->add_money(creature->get_reward());
      m_map->clear_creature(creature->get_coord());
	  itr = m_wave.erase(itr);
	  m_player->add_score(2 * creature->get_reward()); 
      delete creature;
      continue;
    }

    Coord path = m_map->get_next_path(creature);
    if (path.x == m_map->get_end().x && path.y == m_map->get_end().y)
    {
      m_map->clear_creature(creature->get_coord());
      itr = m_wave.erase(itr);
      delete creature;
      m_player->remove_life();
    }
    else
    {
      Coord xy = creature->get_coord();
      creature->set_coord(path);
      m_map->update(dynamic_cast<Model*>(creature), xy);
      ++itr;
    }
    m_gs->game()->restart_timer(true);
    m_gs->game()->change(true);
  }
}

void CreatureManager::next_level()
{
}

void CreatureManager::_cleanup()
{
  list<Creature*>::iterator itr = m_wave.begin();
  while (itr != m_wave.end())
  {
    Creature* creature = (*itr);
    m_wave.erase(itr++);
    delete creature;
  }
}

void CreatureManager::_populate_levels()
{
  // 10 Levels?
  m_levels.resize(10);

  // Level 1
  m_levels[0].push_back(make_pair(1, GOBLIN));
  m_levels[0].push_back(make_pair(1, ORC));
  m_levels[0].push_back(make_pair(1, TROLL));
  m_levels[0].push_back(make_pair(1, TAUREN));


  // Level 2
  m_levels[1].push_back(make_pair(6, GOBLIN));

  // Level 3
  m_levels[2].push_back(make_pair(6, GOBLIN));
  m_levels[2].push_back(make_pair(4, ORC));

  // Level 4
  m_levels[3].push_back(make_pair(10, TAUREN));

  // Level 5
  m_levels[4].push_back(make_pair(20, GOBLIN));

  // Level 6
  m_levels[5].push_back(make_pair(10, ORC));
  m_levels[5].push_back(make_pair(15, TAUREN));
  
  // Level 7
  m_levels[6].push_back(make_pair(7, GOBLIN));
  m_levels[6].push_back(make_pair(7, ORC));
  m_levels[6].push_back(make_pair(7, TROLL));
  m_levels[6].push_back(make_pair(7, TAUREN));

  // Level 8
  m_levels[7].push_back(make_pair(15, GOBLIN));
  m_levels[7].push_back(make_pair(15, ORC));

  // Level 9
  m_levels[8].push_back(make_pair(30, TAUREN));

  // Level 10
  m_levels[0].push_back(make_pair(10, GOBLIN));
  m_levels[0].push_back(make_pair(10, ORC));
  m_levels[0].push_back(make_pair(10, TROLL));
  m_levels[0].push_back(make_pair(10, TAUREN));
}

bool CreatureManager::is_wave_complete()
{
  return m_wave.empty();
}

void CreatureManager::kill_creature(Creature* creature)
{

}

uint32 CreatureManager::num_creatures(CreatureType c) const {
  uint32 total = 0;
  uint32 size = m_levels[current_level].size();
  for (uint32 i = 0; i < size; i++) {
    if (m_levels[current_level][i].second == c)
      total += m_levels[current_level][i].first;
  }
  return total;
}

uint32 CreatureManager::get_current_level() const
{
	return current_level;
}