
#include "Graphics.h"

Graphics::Graphics() :
    m_sprites("sprites.png"),
    m_tiles("tiles.png") {
}

void Graphics::draw_texture(float x, float y, const Texture& texture,
    int sx, int sy, int w, int h, float alpha,
    float scale, bool flip_x, bool flip_y) {
  m_batch.draw(x, y, texture, sx, sy, w, h, alpha, scale, flip_x, flip_y);
}

void Graphics::draw_sprite(float x, float y, const sprites::Sprite& sprite, bool flip_x) {
  m_batch.draw(x - sprite.pivot_x, y - sprite.pivot_y, m_sprites,
    sprite.x, sprite.y, sprite.w, sprite.h, 1, 1, flip_x);
}

void Graphics::draw_tile(int x, int y, const tiles::Tile& tile) {
  m_batch.draw(x * tile_size, y * tile_size,
    m_tiles, tile.x, tile.y, tile_size, tile_size);
}

void Graphics::flush_drawing() {
  m_batch.flush();
}

