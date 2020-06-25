#pragma once

#include "framework/Texture.h"
#include "framework/SpriteBatch.h"
#include "framework/Target.h"
#include "NanoVG.h"
#include "tweakable.h"
#include "choreograph/Choreograph.h"
#include "resources/sprites.h"

class State;
extern NVGcontext* vg;

class View {
public:
  explicit View(const State* state);
  void draw(float frame_pos);

private:
  void draw_sprite(float x, float y, const sprites::Sprite& sprite);
  void do_draw_animation(float x, float y,
    const sprites::Sprite* const* frames, float position);

  template<size_t N>
  void draw_animation(float x, float y,
      const sprites::Sprite* const(&frames)[N], float position) {
    do_draw_animation(x, y, frames, std::fmod(position, N));
  }

  const State& m_state;
  Texture m_sprites;
  Texture m_world;
  SpriteBatch m_batch{ 100 };

  choreograph::Timeline m_timeline;
  choreograph::Output<float> m_radius{ };
};
