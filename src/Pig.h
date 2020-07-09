#pragma once

#include "Actor.h"

class Graphics;

class Pig : public Actor {
public:
  enum class State {
    running,
    grounded,
    attacking,
    hit,
    dead,
  };

  Pig();

  void on_run() override;
  void on_jump() override;
  void on_attack() override;
  void on_grounded() override;
  void on_hit() override;
  void on_interaction(Entity& other) override;
  void update();

  void draw(Graphics& graphics, float frame_pos) const;

protected:
  Pig(EntityType entity_type);
  State state() const { return m_state; }
  float state_counter() const { return m_state_counter; }

private:
  State m_state{ };
  float m_state_counter{ };
};
