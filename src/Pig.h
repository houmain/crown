#pragma once

#include "Actor.h"

class Graphics;

class Pig : public Actor {
public:
  Pig();

  void update();
  void draw(Graphics& graphics, float frame_pos);

protected:
  Pig(EntityType entity_type);

};
