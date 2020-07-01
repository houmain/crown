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
  void apply_input(ActorInput input, bool down);
  bool looking_left() const { return m_looking_left; }
  Object& get_object();

protected:
  explicit Actor(ObjectType object_type);
  void update();

private:
  const Handle m_object_handle;
  int m_input{ };
  bool m_looking_left{ };
};
