
#include "Pig.h"
#include "Graphics.h"

Pig::Pig()
  : Actor(ObjectType::pig) {
}

void Pig::update() {
  Actor::update();
}

void Pig::draw(Graphics& graphics, float frame_pos) {
  static float frame = 0;
  frame += TWEAKABLE(0.05);

  const auto& object = get_object();
  graphics.draw_animation(
    object.get_x_at(frame_pos),
    object.get_y_at(frame_pos),
    sprites::king_pig_idle, frame,
    looking_left());
}
