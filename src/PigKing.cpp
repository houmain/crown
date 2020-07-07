#include "PigKing.h"
#include "Graphics.h"
#include "Object.h"

PigKing::PigKing()
  : Pig(EntityType::pig_king) {
}

void PigKing::draw(Graphics& graphics, float frame_pos) {
  static float frame = 0;
  frame += TWEAKABLE(0.05);

  const auto& object = get_object();
  graphics.draw(
    object.get_x_at(frame_pos),
    object.get_y_at(frame_pos),
    sprites::king_pig_idle, frame,
    looking_left());
}
