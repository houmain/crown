
#include "Actor.h"

Actor::Actor(EntityType entity_type)
  : Entity(entity_type) {
}

void Actor::apply_input(ActorInput input, bool down) {
  if (down) {
    m_input |= static_cast<int>(input);
  }
  else {
    m_input &= ~static_cast<int>(input);
  }
}

void Actor::update_input() {
  if (m_input & static_cast<int>(ActorInput::move_left)) {
    set_looking_left(true);
    on_run();
  }
  if (m_input & static_cast<int>(ActorInput::move_right)) {
    set_looking_left(false);
    on_run();
  }
  if (m_input & static_cast<int>(ActorInput::jump))
    on_jump();

  if (m_input & static_cast<int>(ActorInput::attack))
    on_attack();
}
