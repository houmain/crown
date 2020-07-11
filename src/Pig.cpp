
#include "Pig.h"
#include "Graphics.h"
#include "Object.h"
#include "framework/audio.h"

Pig::Pig()
  : Pig(EntityType::pig) {
}

Pig::Pig(EntityType entity_type)
  : Actor(entity_type) {

  auto& object = get_object();
  object.set_size(14, 16);
  object.set_interaction_radius(200);
}


void Pig::on_run() {
  const auto run_acceleration = TWEAKABLE(0.25f);
  const auto fly_acceleration = TWEAKABLE(0.05f);
  auto& object = get_object();
  object.apply_force((object.on_ground() ? run_acceleration : fly_acceleration) *
    (looking_left() ? -1 : 1), 0);
}

void Pig::on_jump() {
  auto& object = get_object();
  const auto jump_acceleration = TWEAKABLE(4.0f);
  const auto run_jump_acceleration = TWEAKABLE(0.4f);
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
  if (m_state == State::running) {
    if (object.velocity_y() > TWEAKABLE(5.0f)) {
      m_state = State::grounded;
      m_state_counter = TWEAKABLE(2.5f) * object.velocity_y();
    }
  }
  play_audio("swing2.ogg", TWEAKABLE(0.3f));
}

void Pig::on_hit() {
  m_state = State::hit;
  m_state_counter = 0;

  const auto& object = get_object();
  set_looking_left(object.velocity_x() > 0);

  play_audio("grunt1.ogg", TWEAKABLE(0.5f));
}

void Pig::update() {
  auto& object = get_object();
  switch (m_state) {
    case State::running:
      if (!object.on_ground()) {
        // jumping
      }
      else if (object.velocity_x()) {
        // running
        const auto animation = Animation(sprites::pig_run);
        const auto prev_frame_index = animation.get_frame_index(m_state_counter);

        if (std::fabs(object.velocity_x()) > TWEAKABLE(0.25f)) {
          m_state_counter += TWEAKABLE(0.15f) * object.velocity_x();
        }
        else {
          // halting
          m_state_counter = 0;
        }

        const auto frame_index = animation.get_frame_index(m_state_counter);
        if (prev_frame_index != frame_index && (frame_index == 2 || frame_index == 5))
          play_audio("swing3.ogg", TWEAKABLE(0.3f));
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
      m_state_counter += TWEAKABLE(0.2f);
      if (m_state_counter >= 4) {
        m_state = State::running;
        object.set_interaction_radius(0);
      }
      break;

    case State::hit:
      m_state_counter += TWEAKABLE(0.1f);
      if (m_state_counter >= 4) {
        m_state = State::dead;
        m_state_counter = 0;
      }
      break;

    case State::dead:
      m_state_counter += TWEAKABLE(0.1f);
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
      else if (std::fabs(object.velocity_x()) > TWEAKABLE(0.25f)) {
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
