#pragma once

#include "Actor.h"

class Graphics;

class Player : public Actor {
public:
  Player();

  void update();
  void draw(Graphics& graphics, float frame_pos);
};
