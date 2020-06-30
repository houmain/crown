
#include "Actor.h"
#include "Game.h"

Actor::Actor(Object* object)
  : m_object(*object) {

  m_object.set_size(20, 26);
}

void Actor::apply_input(ActorInput input, bool down) {
  if (down)
    m_input |= static_cast<int>(input);
  else
    m_input &= ~static_cast<int>(input);
}

void Actor::update() {
  const auto run_acceleration = TWEAKABLE(0.4);
  const auto fly_acceleration = TWEAKABLE(0.2);
  const auto jump_acceleration = TWEAKABLE(3.0);
  const auto run_jump_acceleration = TWEAKABLE(0.3);

  if (m_input & static_cast<int>(ActorInput::move_left)) {
    m_object.apply_force(-(m_object.on_ground() ? run_acceleration : fly_acceleration), 0);
    m_looking_left = true;
  }
  if (m_input & static_cast<int>(ActorInput::move_right)) {
    m_object.apply_force((m_object.on_ground() ? run_acceleration : fly_acceleration), 0);
    m_looking_left = false;
  }
  if (m_input & static_cast<int>(ActorInput::jump))
    if (m_object.on_ground())
      m_object.apply_force(0, -(jump_acceleration + run_jump_acceleration * std::fabs(m_object.velocity_x())));
}
