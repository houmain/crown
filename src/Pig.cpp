
#include "Pig.h"
#include "Graphics.h"
#include "Object.h"

Pig::Pig()
  : Pig(EntityType::pig) {
}

Pig::Pig(EntityType entity_type)
  : Actor(entity_type) {

  get_object().set_size(18, 18);
}

void Pig::update() {
  Actor::update();
}

void Pig::draw(Graphics& graphics, float frame_pos) {
  static float frame = 0;
  frame += TWEAKABLE(0.05);

  const auto& object = get_object();
  graphics.draw(
    object.get_x_at(frame_pos),
    object.get_y_at(frame_pos),
    sprites::pig_idle, frame,
    looking_left());
}
