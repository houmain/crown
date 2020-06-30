#pragma once

#include "Object.h"

enum class ActorInput {
  move_left  = (1 << 0),
  move_right = (1 << 1),
  jump       = (1 << 2),
  attack     = (1 << 3),
};

class Actor {
public:
  explicit Actor(Object* object);

  void apply_input(ActorInput input, bool down);
  void update();
  Object& object() { return m_object; }
  bool looking_left() const { return m_looking_left; }

private:
  Object& m_object;
  int m_input{ };
  bool m_looking_left{ };
};
