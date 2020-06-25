
#include "Target.h"
#include "ShaderProgram.h"

namespace {
  int g_target_width;
  int g_target_height;
  float g_target_scale;
  ShaderProgram g_program;

  void bind_target(GLuint fbo, int width, int height, float scale) {
    static GLuint s_current;
    if (std::exchange(s_current, fbo) != fbo)
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, width, height);
    g_target_width = width;
    g_target_height = height;
    g_target_scale = scale;
  }
} // namespace

void bind_default_target(int width, int height, float scale) {
  bind_target(GL_NONE, width, height, scale);
}

int target_width() { return g_target_width / g_target_scale; }
int target_height() { return g_target_height / g_target_scale; }
float target_scale() { return g_target_scale; }

void Target::bind(int width, int height, float scale, TextureFormat format) {
  if (m_fbo &&
      width == m_texture.width() &&
      height == m_texture.height() &&
      format == m_texture.format())
    return bind_target(m_fbo, width, height, scale);

  auto object = GLuint{};
  glGenFramebuffers(1, &object);
  m_fbo = { object, [](GLuint fbo) { glDeleteFramebuffers(1, &fbo); }};
  bind_target(object, width, height, scale);

  glGenRenderbuffers(1, &object);
  m_rbo = { object, [](GLuint rbo) { glDeleteRenderbuffers(1, &rbo); }};
  glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
    GL_RENDERBUFFER, m_rbo);

  m_texture = Texture(format, width, height);
  m_texture.bind(0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
    m_texture.texture_object(), 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    platform_error("creating framebuffer failed");
}
