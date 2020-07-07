#pragma once

#include "Actor.h"

class Graphics;

class Player final : public Actor {
public:
  enum class State {
    running,
    grounded,
    attacking,
    hit,
    dead,
    entering,
    leaving,
  };

  Player();

  void update();
  void draw(Graphics& graphics, float frame_pos);

  void on_grounded() override;

private:
  State m_state{ };
  float m_state_counter{ };
};
