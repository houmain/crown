#pragma once

#include "Entity.h"

enum class ActorInput {
  move_left  = (1 << 0),
  move_right = (1 << 1),
  jump       = (1 << 2),
  attack     = (1 << 3),
};

class Actor : public Entity {
public:
  void apply_input(ActorInput input, bool down);
  bool looking_left() const { return m_looking_left; }

protected:
  Actor(EntityType entity_type);
  void update();

private:
  int m_input{ };
  bool m_looking_left{ };
};
