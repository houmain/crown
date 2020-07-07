
#include "Player.h"
#include "Graphics.h"
#include "Object.h"

Player::Player()
  : Actor(EntityType::player) {

  get_object().set_size(14, 26);
}

void Player::update() {
  if (m_state == State::running)
    Actor::update();
}

void Player::on_grounded() {
  auto& object = get_object();
  if (object.velocity_y() > TWEAKABLE(5)) {
    m_state = State::grounded;
    m_state_counter = TWEAKABLE(2.5) * object.velocity_y() ;
  }
  object.apply_force(0, -object.velocity_y() / 2);
}

void Player::draw(Graphics& graphics, float frame_pos) {
  auto animation = Animation(sprites::player_idle);
  const auto& object = get_object();
  switch (m_state) {
    case State::running: {
      if (!object.on_ground()) {
        animation = (object.velocity_y() < 0 ?
          sprites::player_jump : sprites::player_fall);
      }
      else if (std::fabs(object.velocity_x()) > TWEAKABLE(1.0)) {
        animation = sprites::player_run;
        m_state_counter += TWEAKABLE(0.15) * object.velocity_x();
      }
      else {
        animation = sprites::player_idle;
        m_state_counter += TWEAKABLE(0.1);
      }
      break;
    }

    case State::grounded:
      animation = sprites::player_ground;
      m_state_counter -= 1;
      if (m_state_counter <= 0)
        m_state = State::running;
      break;

    case State::attacking:
      animation = sprites::player_attack;
      if (m_state_counter <= 0)
        m_state = State::running;
      break;

    case State::hit:
      animation = sprites::player_hit;
      if (m_state_counter <= 0)
        m_state = State::running;
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
