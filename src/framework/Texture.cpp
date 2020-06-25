
#include "Texture.h"
#include "ImageFile.h"
#include <cassert>

namespace {
  struct TextureFormatDesc {
    GLenum internal_format;
    GLenum format;
    int components;
    GLenum data_type;
    size_t data_size;
  };

  TextureFormatDesc get_desc(TextureFormat type) {
    switch (type) {
      case TextureFormat::R8:  return { GL_RED, GL_RED, 1, GL_UNSIGNED_BYTE, 1 };
      case TextureFormat::RG8:  return { GL_RG, GL_RG, 2, GL_UNSIGNED_BYTE, 2 };
      case TextureFormat::RGB8:  return { GL_RGB, GL_RGB, 3, GL_UNSIGNED_BYTE, 3 };
      case TextureFormat::RGBA8:  return { GL_RGBA, GL_RGBA, 4, GL_UNSIGNED_BYTE, 4 };
    }
    return { };
  }
} // namespace

Texture::Texture(std::string_view filename) {
  auto image = ImageFile(filename);
  switch (image.components()) {
    case 1: m_format = TextureFormat::R8; break;
    case 2: m_format = TextureFormat::RG8; break;
    case 3: m_format = TextureFormat::RGB8; break;
    case 4: m_format = TextureFormat::RGBA8; break;
  }
  m_width = image.width();
  m_height = image.height();
  create(image.data());
}

Texture::Texture(TextureFormat format, int width, int height)
  : m_format(format), m_width(width), m_height(height) {
  create(nullptr);
}

void Texture::create(const void* data) {
  auto obj = GLuint{ };
  glGenTextures(1, &obj);
  m_texture_object = { obj, [](GLuint obj) { glDeleteTextures(1, &obj); } };

  const auto desc = get_desc(m_format);
  glBindTexture(GL_TEXTURE_2D, m_texture_object);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, desc.internal_format, m_width,
    m_height, 0, desc.format, desc.data_type, data);
  glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Texture::bind(int unit) const {
  static int s_current_unit;
  if (std::exchange(s_current_unit, unit) != unit)
    glActiveTexture(GL_TEXTURE0 + unit);

  glBindTexture(GL_TEXTURE_2D, m_texture_object);
}

void Texture::filter_linear() {
  glBindTexture(GL_TEXTURE_2D, m_texture_object);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::upload(const void* data, size_t size) {
  glBindTexture(GL_TEXTURE_2D, m_texture_object);
  const auto desc = get_desc(m_format);
  assert(size == m_width * m_height * desc.data_size);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height,
    desc.format, desc.data_type, data);
  glBindTexture(GL_TEXTURE_2D, GL_NONE);
}
