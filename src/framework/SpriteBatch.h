#pragma once

#include "ShaderProgram.h"

class Texture;

class SpriteBatch {
public:
  explicit SpriteBatch(size_t capacity = 32);

  void draw(float x, float y,
    const Texture& texture, int sx, int sy, int w, int h,
    float alpha = 1, float scale = 1,
    bool flip_s = false, bool flip_t = false);

  void flush();

private:
  struct Vertex {
    float x, y, s, t, alpha;
  };
  struct Rect { Vertex v0, v1, v2, v3, v4, v5; };
  static_assert(sizeof(Rect) == 6 * sizeof(Vertex), "wrong");

  void initialize();

  ShaderProgram m_program;
  Resource<GLuint> m_vbo;
  std::vector<std::pair<size_t, const Texture*>> m_texture_switch;
  std::vector<Rect> m_client_buffer;
};
