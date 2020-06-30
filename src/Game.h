#pragma once

#include "choreograph/Choreograph.h"
#include "Graphics.h"
#include "World.h"
#include "Actor.h"

extern class Game* game;

class Game : public World {
public:
  Game();

  double update_interval() const { return 1 / 60.0; }

  void update(double time);
  void draw();

  Actor& get_player() { return m_player; }

private:
  void do_update();

  // state
  choreograph::Timeline m_update_timeline;
  double m_update_time{ };
  double m_previous_time{ };
  std::vector<Object> m_objects;
  Actor m_player;

  // view
  choreograph::Timeline m_draw_timeline;
};
