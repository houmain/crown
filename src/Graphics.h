#pragma once

#include "framework/Texture.h"
#include "framework/SpriteBatch.h"
#include "framework/Target.h"
#include "resources/sprites.h"
#include "NanoVG.h"

class Animation {
public:
  template<size_t N>
  Animation(const sprites::Sprite* const(&frames)[N])
    : m_frames(frames), m_count(N) {
  }

  const sprites::Sprite& get_frame(float position) const {
    if (m_backwards) {
      position = m_count - 1 - position;
    }
    if (m_clamping) {
      position = std::min(std::max(position, 0.0f), static_cast<float>(m_count - 1));
    }
    return *m_frames[(m_count + static_cast<int>(position) % m_count) % m_count];
  }
  void backwards() { m_backwards = true; }
  void clamp() { m_clamping = true; }

private:
  const sprites::Sprite* const* m_frames;
  int m_count;
  bool m_backwards{ };
  bool m_clamping{ };
};

class Graphics {
public:
  Graphics();

  void enable_blending();
  void disable_blending();

  void draw(float x, float y, const Texture& texture,
      int sx, int sy, int w, int h, float alpha = 1.0f,
      float scale = 1.0f, bool flip_x = false, bool flip_y = false);

  void draw(float x, float y, const sprites::Sprite& sprite, bool flip_x = false);

  void draw(float x, float y, Animation animation, float position, bool flip_x = false) {
    draw(x, y, animation.get_frame(position), flip_x);
  }

  void flush_drawing();

private:
  const Texture m_sprites;
  SpriteBatch m_batch{ 100 };
  bool m_blending{ };
};
