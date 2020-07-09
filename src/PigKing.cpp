#include "PigKing.h"
#include "Graphics.h"
#include "Object.h"

PigKing::PigKing()
  : Pig(EntityType::pig_king) {
}

void PigKing::draw(Graphics& graphics, float frame_pos) const {
  auto animation = Animation(sprites::king_pig_idle);
  const auto& object = get_object();
  switch (state()) {
    case State::running:
      if (!object.on_ground()) {
        animation = (object.velocity_y() < 0 ?
          sprites::king_pig_jump : sprites::king_pig_fall);
      }
      else if (std::fabs(object.velocity_x()) > TWEAKABLE(0.25)) {
        animation = sprites::king_pig_run;
      }
      else {
        animation = sprites::king_pig_idle;
      }
      break;

    case State::grounded:
      animation = sprites::king_pig_ground;
      break;

    case State::attacking:
      animation = sprites::king_pig_attack;
      animation.clamp();
      break;

    case State::hit:
      animation = sprites::king_pig_hit;
      animation.clamp();
      break;

    case State::dead:
      animation = sprites::king_pig_dead;
      animation.clamp();
      break;
  }

  graphics.draw(
    object.get_x_at(frame_pos),
    object.get_y_at(frame_pos),
    animation, state_counter(),
    looking_left());
}
