
#include "SpriteBatch.h"
#include "Texture.h"
#include "Target.h"

SpriteBatch::SpriteBatch(size_t capacity) {
  m_client_buffer.reserve(capacity);
}

void SpriteBatch::initialize() {
  const auto vs = R"(
#version 100
precision highp float;

attribute vec4 a_position_texcoords;
attribute float a_alpha;
varying vec2 v_texcoords;
varying float v_alpha;

void main() {
  v_texcoords = a_position_texcoords.zw;
  v_alpha = a_alpha;
  gl_Position = vec4(a_position_texcoords.xy, 0, 1);
}
)";

  const auto fs = R"(
#version 100
precision highp float;

uniform sampler2D u_texture;
varying vec2 v_texcoords;
varying float v_alpha;

void main() {
  vec4 color = texture2D(u_texture, v_texcoords);
  // premultiply
  gl_FragColor = color * (color.a * v_alpha);
}
)";

  auto vbo = GLuint{ };
  glGenBuffers(1, &vbo);
  m_vbo = { vbo, [](GLuint vbo) { glDeleteBuffers(1, &vbo); } };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
    m_client_buffer.capacity() * sizeof(Rect), NULL, GL_STREAM_DRAW);

  m_program = ShaderProgram(vs, fs, { "a_position_texcoords", "a_alpha" });
  m_program.bind();
  glUniform1i(glGetUniformLocation(m_program, "u_texture"), 0);
}

void SpriteBatch::draw(float x, float y, const Texture& texture,
    int sx, int sy, int w, int h, float alpha,
    float scale, bool flip_s, bool flip_t) {

  if (m_client_buffer.size() == m_client_buffer.capacity())
    flush();

  if (m_texture_switch.empty() ||
      m_texture_switch.back().second != &texture)
    m_texture_switch.emplace_back(m_client_buffer.size(), &texture);

  // round to integral coordinates
  x = std::floor(x * target_scale() + 0.5f) / target_scale();
  y = std::floor(y * target_scale() + 0.5f) / target_scale();

  auto x0 = x / target_width() * 2.0f - 1;
  auto y0 = (target_height() - y * 2.0f) / target_height();
  auto x1 = x0 + w * 2.0f * scale / target_width();
  auto y1 = y0 - h * 2.0f * scale / target_height();
  auto s0 = static_cast<float>(sx) / texture.width();
  auto t0 = static_cast<float>(sy) / texture.height();
  auto s1 = static_cast<float>(sx + w) / texture.width();
  auto t1 = static_cast<float>(sy + h) / texture.height();
  if (flip_s)
    std::swap(s0, s1);
  if (flip_t)
    std::swap(t0, t1);

  m_client_buffer.push_back({
    Vertex{ x0, y0, s0, t0, alpha },
    Vertex{ x0, y1, s0, t1, alpha },
    Vertex{ x1, y0, s1, t0, alpha },
    Vertex{ x1, y1, s1, t1, alpha },
    Vertex{ x1, y0, s1, t0, alpha },
    Vertex{ x0, y1, s0, t1, alpha },
  });
}

void SpriteBatch::flush() {
  if (m_client_buffer.empty())
    return;

  if (!m_vbo)
    initialize();

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
#if 0 // orphan
  glBufferData(GL_ARRAY_BUFFER,
    m_client_buffer.capacity() * sizeof(Rect), NULL, GL_STREAM_DRAW);
#endif
  glBufferSubData(GL_ARRAY_BUFFER, 0,
    m_client_buffer.size() * sizeof(Rect), m_client_buffer.data());

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), (void*)offsetof(Vertex, x));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), (void*)offsetof(Vertex, alpha));

  m_program.bind();

  auto prev_offset = size_t{ };
  for (const auto& offset_texture : m_texture_switch) {
    const auto offset = offset_texture.first;
    if (offset != prev_offset)
      glDrawArrays(GL_TRIANGLES, static_cast<GLint>(prev_offset) * 6, 
        static_cast<GLsizei>(offset - prev_offset) * 6);
    offset_texture.second->bind(0);
    prev_offset = offset;
  }
  glDrawArrays(GL_TRIANGLES, static_cast<GLint>(prev_offset) * 6, 
    static_cast<GLsizei>(m_client_buffer.size() - prev_offset) * 6);
  m_client_buffer.clear();
  m_texture_switch.clear();
}
