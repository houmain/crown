#pragma once

#include "Graphics.h"
#include "World.h"
#include "Player.h"
#include "Pig.h"
#include "unordered_vector.h"
#include "choreograph/Choreograph.h"

extern class Game* game;

class Game {
public:
  static void instantiate();

  double update_interval() const { return 1 / 60.0; }

  void update(double time);
  void draw();

  Handle allocate_object(ObjectType object_type);
  Object& get_object(Handle handle) { return m_objects[handle]; }
  Player& player() { return m_player; }
  World& world() { return m_world; }

private:
  Game();
  void do_update();

  Graphics m_graphics;
  World m_world;

  // state
  choreograph::Timeline m_update_timeline;
  double m_update_time{ };
  double m_previous_time{ };
  unordered_vector<Object, Handle> m_objects;
  unordered_vector<Pig, Handle> m_pigs;
  Player m_player;

  // view
  choreograph::Timeline m_draw_timeline;
};
