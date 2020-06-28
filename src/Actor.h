#pragma once

#include "Object.h"

enum class ActorInput {
  move_left,
  move_right,
  jump,
  fire,
};

class Actor {
public:
  Actor(Object* object);

  void apply_input(ActorInput input, bool down);
  void update();

private:
  Object& m_object;
};
