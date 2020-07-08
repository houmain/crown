#pragma once

#include "Graphics.h"
#include "World.h"
#include "Player.h"
#include "Pig.h"
#include "PigKing.h"
#include "Object.h"
#include "unordered_vector.h"
#include "choreograph/Choreograph.h"

extern class Game* game;

class Game {
public:
  static void instantiate();

  double update_interval() const { return 1 / TWEAKABLE(60.0); }

  void update(double time);
  void draw();

  Handle allocate_object(EntityType entity_type);
  Pig& create_pig(float x, float y);
  Object& get_object(Handle object_handle) { return m_objects[object_handle]; }
  Entity& get_entity(EntityType entity_type, Handle entity_handle);
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
  PigKing m_pig_king;

  // view
  choreograph::Timeline m_draw_timeline;
};
