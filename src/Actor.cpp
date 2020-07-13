
#include "Actor.h"

Actor::Actor(EntityType entity_type)
  : Entity(entity_type) {
}

void Actor::apply_input(ActorInput input, bool down) {
  if (down) {
    m_input_down |= static_cast<int>(input);
  }
  else {
    m_input_up |= static_cast<int>(input);
  }
}

void Actor::update_input() {
  m_input_hold |= m_input_down;
  m_input_hold &= ~m_input_up;

  const auto hold_or_down = [&](ActorInput input) {
    return (((m_input_hold | m_input_down) & static_cast<int>(input)) != 0);
  };

  if (hold_or_down(ActorInput::move_left)) {
    set_looking_left(true);
    on_run();
  }
  if (hold_or_down(ActorInput::move_right)) {
    set_looking_left(false);
    on_run();
  }
  if (hold_or_down(ActorInput::jump))
    on_jump();

  if (hold_or_down(ActorInput::attack))
    on_attack();

  m_input_down = { };
  m_input_up = { };
}
