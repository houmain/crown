#pragma once

#include "Pig.h"

class PigKing final : public Pig {
public:
  PigKing();

  void draw(Graphics& graphics, float frame_pos);
};
