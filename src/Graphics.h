#pragma once

#include "framework/Texture.h"
#include "framework/SpriteBatch.h"
#include "framework/Target.h"
#include "resources/sprites.h"
#include "resources/tiles.h"
#include "tweakable.h"
#include "NanoVG.h"

class Graphics {
public:
  static constexpr int tile_size = 32;

  Graphics();

  void draw_texture(float x, float y, const Texture& texture,
      int sx, int sy, int w, int h, float alpha = 1.0f,
      float scale = 1.0f, bool flip_s = false, bool flip_t = false);

  void draw_sprite(float x, float y, const sprites::Sprite& sprite);

  template<size_t N>
  void draw_animation(float x, float y,
      const sprites::Sprite* const(&frames)[N], float position) {
    do_draw_animation(x, y, frames, std::fmod(position, static_cast<float>(N)));
  }

  void draw_tile(int x, int y, const tiles::Tile& tile);

  void flush_drawing();

private:
  void do_draw_animation(float x, float y,
      const sprites::Sprite* const* frames, float position);

  Texture m_sprites;
  Texture m_tiles;
  SpriteBatch m_batch{ 100 };
};
