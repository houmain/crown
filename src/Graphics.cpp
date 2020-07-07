
#include "Graphics.h"

Graphics::Graphics() :
    m_sprites("sprites.png") {
  glDisable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void Graphics::enable_blending() {
  if (!std::exchange(m_blending, true)) {
    flush_drawing();
    glEnable(GL_BLEND);
  }
}

void Graphics::disable_blending() {
  if (std::exchange(m_blending, false)) {
    flush_drawing();
    glDisable(GL_BLEND);
  }
}

void Graphics::draw(float x, float y, const Texture& texture,
    int sx, int sy, int w, int h, float alpha,
    float scale, bool flip_x, bool flip_y) {
  m_batch.draw(x, y, texture, sx, sy, w, h, alpha, scale, flip_x, flip_y);
}

void Graphics::draw(float x, float y, const sprites::Sprite& sprite, bool flip_x) {
  m_batch.draw(
    (!flip_x ? x - sprite.pivot_x : x - sprite.w + sprite.pivot_x),
    y - sprite.pivot_y, m_sprites,
    sprite.x, sprite.y, sprite.w, sprite.h, 1, 1, flip_x);
}

void Graphics::flush_drawing() {
  m_batch.flush();
}

