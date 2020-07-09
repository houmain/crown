
#include "Pig.h"
#include "Graphics.h"
#include "Object.h"

Pig::Pig()
  : Pig(EntityType::pig) {
}

Pig::Pig(EntityType entity_type)
  : Actor(entity_type) {

  auto& object = get_object();
  object.set_size(18, 18);
  object.set_interaction_radius(200);
}


void Pig::on_run() {
  const auto run_acceleration = TWEAKABLE(0.25);
  const auto fly_acceleration = TWEAKABLE(0.05);
  auto& object = get_object();
  object.apply_force((object.on_ground() ? run_acceleration : fly_acceleration) *
    (looking_left() ? -1 : 1), 0);
}

void Pig::on_jump() {
  auto& object = get_object();
  const auto jump_acceleration = TWEAKABLE(4.0);
  const auto run_jump_acceleration = TWEAKABLE(0.4);
  if (object.on_ground())
    object.apply_force(0, -(jump_acceleration + run_jump_acceleration * std::fabs(object.velocity_x())));
}

void Pig::on_attack() {
  if (m_state == State::running) {
    m_state = State::attacking;
    m_state_counter = 0;
  }
}

void Pig::on_grounded() {
  const auto& object = get_object();
  if (object.velocity_y() > TWEAKABLE(5)) {
    m_state = State::grounded;
    m_state_counter = TWEAKABLE(2.5) * object.velocity_y();
  }
}

void Pig::on_hit() {
  m_state = State::hit;
  m_state_counter = 0;

  const auto& object = get_object();
  set_looking_left(object.velocity_x() < 0);
}

void Pig::update() {
  auto& object = get_object();
  switch (m_state) {
    case State::running:
      if (!object.on_ground()) {
        // jumping
      }
      else if (std::fabs(object.velocity_x()) > TWEAKABLE(0.25)) {
        // running
        m_state_counter += TWEAKABLE(0.15) * object.velocity_x();
      }
      else {
        // idle
        m_state_counter += TWEAKABLE(0.1);
      }
      update_input();
      break;

    case State::grounded:
      m_state_counter -= 1;
      if (m_state_counter <= 0)
        m_state = State::running;
      break;

    case State::attacking:
      m_state_counter += TWEAKABLE(0.2);
      if (m_state_counter >= 4) {
        m_state = State::running;
        object.set_interaction_radius(0);
      }
      break;

    case State::hit:
      m_state_counter += TWEAKABLE(0.1);
      if (m_state_counter >= 4) {
        m_state = State::dead;
        m_state_counter = 0;
      }
      break;

    case State::dead:
      m_state_counter += TWEAKABLE(0.1);
      break;
  }
}

void Pig::draw(Graphics& graphics, float frame_pos) const {
  auto animation = Animation(sprites::pig_idle);
  const auto& object = get_object();
  switch (m_state) {
    case State::running:
      if (!object.on_ground()) {
        animation = (object.velocity_y() < 0 ?
          sprites::pig_jump : sprites::pig_fall);
      }
      else if (std::fabs(object.velocity_x()) > TWEAKABLE(0.25)) {
        animation = sprites::pig_run;
      }
      else {
        animation = sprites::pig_idle;
      }
      break;

    case State::grounded:
      animation = sprites::pig_ground;
      break;

    case State::attacking:
      animation = sprites::pig_attack;
      animation.clamp();
      break;

    case State::hit:
      animation = sprites::pig_hit;
      animation.clamp();
      break;

    case State::dead:
      animation = sprites::pig_dead;
      animation.clamp();
      break;
  }

  graphics.draw(
    object.get_x_at(frame_pos),
    object.get_y_at(frame_pos),
    animation, m_state_counter,
    !looking_left());
}

void Pig::on_interaction(Entity& other) {
  auto& object = other.get_object();
  if (m_state == State::attacking && !m_state_counter) {
    if (looking_left())
      object.apply_force(-5, -5);
    else
      object.apply_force(5, -5);

    other.on_hit();
  }
}
