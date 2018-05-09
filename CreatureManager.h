#ifndef CreatureManager_h__
#define CreatureManager_h__

#include "Config.h"
#include "game_state.h"
#include "map.h"
#include "creature.h"

class GameState;
class CreatureInfoView;

// Manages the creature waves.
class CreatureManager
{
public:
  explicit CreatureManager(Map* map, GameState* gs, Player* player);

  // Sets up the current wave based on the current level.
  void setup();
  // Advances the wave by one tile.
  void advance();
  // TODO: setup next level.
  void next_level();
  // Returns true is the wave is complete.
  bool is_wave_complete();
  void kill_creature(Creature* creature);

  uint32 num_creatures(CreatureType) const;
  uint32 get_current_level() const;

  friend class CreatureInfoView;
private:
  void _create_wave();
  void _populate_levels();
  void _cleanup();

  Map* m_map;
  GameState* m_gs;
  Player* m_player;
  list<Creature*> m_wave;
  uint32 current_level;
  vector<vector<pair<uint32, CreatureType> > > m_levels;
};

#endif // CreatureManager_h__
