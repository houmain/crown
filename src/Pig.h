#pragma once

#include "Actor.h"

class Graphics;

class Pig : public Actor {
public:
  explicit Pig();

  void update();
  void draw(Graphics& graphics, float frame_pos);
};
