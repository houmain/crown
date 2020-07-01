
#include "Player.h"
#include "Graphics.h"

Player::Player()
  : Actor(ObjectType::player) {

  get_object().set_size(20, 26);
}

void Player::update() {
  Actor::update();
}

void Player::draw(Graphics& graphics, float frame_pos) {
  static float frame = 0;
  const auto& object = get_object();
  frame += object.velocity_x() * TWEAKABLE(0.15);

  if (object.velocity_x()) {
    graphics.draw_animation(
      object.get_x_at(frame_pos),
      object.get_y_at(frame_pos),
      sprites::player_run, frame,
      looking_left());
  }
  else {
    graphics.draw_animation(
      object.get_x_at(frame_pos),
      object.get_y_at(frame_pos),
      sprites::player_idle, 0,
      looking_left());
  }
}
