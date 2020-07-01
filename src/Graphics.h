#pragma once

#include "framework/Texture.h"
#include "framework/SpriteBatch.h"
#include "framework/Target.h"
#include "resources/sprites.h"
#include "NanoVG.h"

class Graphics {
public:
  Graphics();

  void enable_blending();
  void disable_blending();

  void draw_texture(float x, float y, const Texture& texture,
      int sx, int sy, int w, int h, float alpha = 1.0f,
      float scale = 1.0f, bool flip_x = false, bool flip_y = false);

  void draw_sprite(float x, float y, const sprites::Sprite& sprite, bool flip_x = false);

  template<size_t N>
  void draw_animation(float x, float y,
      const sprites::Sprite* const(&frames)[N], float position, bool flip_x = false) {
    const auto number = (N + static_cast<int>(position) % N) % N;
    draw_sprite(x, y, *frames[number], flip_x);
  }

  void flush_drawing();

private:
  const Texture m_sprites;
  SpriteBatch m_batch{ 100 };
  bool m_blending{ };
};
