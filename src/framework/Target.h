#pragma once

#include "Texture.h"

class Target {
public:
  void bind(int width, int height, float scale = 1.0f, TextureFormat format = TextureFormat::RGBA8);
  const Texture& texture() const { return m_texture; }
  int width() const { return m_texture.width(); }
  int height() const { return m_texture.height(); }
  void filter_linear() { m_texture.filter_linear(); }

  operator Texture() && { return std::move(m_texture); };

private:
  Resource<GLuint> m_fbo;
  Resource<GLuint> m_rbo;
  Texture m_texture;
};

void bind_default_target(int width, int height, float scale = 1.0f);
int target_width();
int target_height();
float target_scale();
