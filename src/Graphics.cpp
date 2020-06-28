
#include "Graphics.h"

Graphics::Graphics() :
    m_sprites("sprites.png"),
    m_tiles("tiles.png") {
}

void Graphics::draw_texture(float x, float y, const Texture& texture,
    int sx, int sy, int w, int h, float alpha,
    float scale, bool flip_s, bool flip_t) {
  m_batch.draw(x, y, texture, sx, sy, w, h, alpha, scale, flip_s, flip_t);
}

void Graphics::draw_sprite(float x, float y, const sprites::Sprite& sprite) {
  m_batch.draw(x - sprite.pivot_x, y - sprite.pivot_y, m_sprites,
    sprite.x, sprite.y, sprite.w, sprite.h);
}

void Graphics::draw_tile(int x, int y, const tiles::Tile& tile) {
  m_batch.draw(x, y, m_tiles, tile.x, tile.y, tile_size, tile_size);
}

void Graphics::do_draw_animation(float x, float y,
    const sprites::Sprite* const* frames, float position) {
  const auto& frame = *frames[static_cast<int>(position)];
  draw_sprite(x, y, frame);
}

void Graphics::flush_drawing() {
  m_batch.flush();
}

