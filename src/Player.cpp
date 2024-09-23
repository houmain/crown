
#include "Player.h"
#include "Graphics.h"
#include "Object.h"
#include "framework/audio.h"
#include "_generated/assets.h"

Player::Player()
  : Actor(EntityType::player) {

  auto& object = get_object();
  object.set_size(14, 26);
}

void Player::on_run() {
  const auto run_acceleration = TWEAKABLE(0.3f);
  const auto fly_acceleration = TWEAKABLE(0.05f);
  auto& object = get_object();
  object.apply_force((object.on_ground() ? 
    static_cast<float>(run_acceleration) : fly_acceleration) *
    (looking_left() ? -1 : 1), 0);
}

void Player::on_jump() {
  auto& object = get_object();
  const auto jump_acceleration = TWEAKABLE(3.0f);
  const auto jump_run_acceleration = TWEAKABLE(0.45f);
  const auto jump_hold_acceleration = TWEAKABLE(0.075f);
  if (object.on_ground()) {
    object.apply_force(0, -(jump_acceleration + jump_run_acceleration *
      std::fabs(object.velocity_x())));

    play_audio(assets::sounds::swing3, TWEAKABLE(0.6f));
  }
  else if (object.velocity_y() < 0) {
    object.apply_force(0, -jump_hold_acceleration);
  }
}

void Player::on_attack() {
  if (m_state == State::running) {
    auto& object = get_object();
    m_state = State::attacking;
    m_state_counter = 0;
    object.set_interaction_radius(50);

    if (object.on_ground())
      object.apply_force(TWEAKABLE(-0.5f) * (looking_left() ? 1 : -1), 0);

    play_audio(assets::sounds::swing, TWEAKABLE(1.0f));
  }
}

void Player::on_grounded() {
  const auto& object = get_object();
  if (object.velocity_y() > TWEAKABLE(6.0f)) {
    m_state = State::grounded;
    m_state_counter = TWEAKABLE(2.5f) * object.velocity_y();

    play_audio(assets::sounds::swing2, TWEAKABLE(0.7f));
  }
  else {
    play_audio(assets::sounds::swing2, TWEAKABLE(0.3f));
  }
}

void Player::on_hit() {
  m_state = State::hit;
  m_state_counter = 0;
}

void Player::update() {
  auto& object = get_object();
  switch (m_state) {
    case State::running:
      if (!object.on_ground()) {
        // jumping
      }
      else if (object.velocity_x()) {
        // running
        const auto animation = Animation(sprites::player_run);
        const auto prev_frame_index = animation.get_frame_index(m_state_counter);

        if (std::fabs(object.velocity_x()) > TWEAKABLE(0.25f)) {
          m_state_counter += TWEAKABLE(0.15f) * object.velocity_x();
        }
        else {
          // halting
          m_state_counter = 0;
        }

        const auto frame_index = animation.get_frame_index(m_state_counter);
        if (prev_frame_index != frame_index && (frame_index == 3 || frame_index == 7))
          play_audio(assets::sounds::swing3, TWEAKABLE(0.3f));
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
        m_state = State::running;
      }
      break;

    case State::dead:
      break;

    case State::entering:
      break;

    case State::leaving:
      break;
  }
}

void Player::draw(Graphics& graphics, float frame_pos) const {
  auto animation = Animation(sprites::player_idle);
  const auto& object = get_object();
  switch (m_state) {
    case State::running:
      if (!object.on_ground()) {
        animation = (object.velocity_y() < 0 ?
          sprites::player_jump : sprites::player_fall);
      }
      else if (std::fabs(object.velocity_x()) > TWEAKABLE(0.25f)) {
        animation = sprites::player_run;
      }
      else {
        animation = sprites::player_idle;
      }
      break;

    case State::grounded:
      animation = sprites::player_ground;
      break;

    case State::attacking:
      animation = sprites::player_attack;
      animation.clamp();
      break;

    case State::hit:
      animation = sprites::player_hit;
      break;

    case State::dead:
      animation = sprites::player_dead;
      break;

    case State::entering:
      animation = sprites::player_door_in;
      break;

    case State::leaving:
      animation = sprites::player_door_out;
      break;
  }

  graphics.draw(
    object.get_x_at(frame_pos),
    object.get_y_at(frame_pos),
    animation, m_state_counter,
    looking_left());
}

void Player::on_interaction(Entity& other) {
  auto& object = other.get_object();
  if (m_state == State::attacking && !m_state_counter) {
    if (looking_left())
      object.apply_force(-5, -5);
    else
      object.apply_force(5, -5);

    other.on_hit();
  }
}
