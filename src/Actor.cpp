
#include "Actor.h"
#include "Game.h"

Actor::Actor(ObjectType object_type)
  : m_object_handle(game->allocate_object(object_type)) {
}

Object& Actor::get_object() {
  return game->get_object(m_object_handle);
}

void Actor::apply_input(ActorInput input, bool down) {
  if (down)
    m_input |= static_cast<int>(input);
  else
    m_input &= ~static_cast<int>(input);
}

void Actor::update() {
  const auto run_acceleration = TWEAKABLE(0.4);
  const auto fly_acceleration = TWEAKABLE(0.05);
  const auto jump_acceleration = TWEAKABLE(3.0);
  const auto run_jump_acceleration = TWEAKABLE(0.3);

  auto& object = get_object();
  if (m_input & static_cast<int>(ActorInput::move_left)) {
    object.apply_force(-(object.on_ground() ? run_acceleration : fly_acceleration), 0);
    m_looking_left = true;
  }
  if (m_input & static_cast<int>(ActorInput::move_right)) {
    object.apply_force((object.on_ground() ? run_acceleration : fly_acceleration), 0);
    m_looking_left = false;
  }
  if (m_input & static_cast<int>(ActorInput::jump))
    if (object.on_ground())
      object.apply_force(0, -(jump_acceleration + run_jump_acceleration * std::fabs(object.velocity_x())));
}
