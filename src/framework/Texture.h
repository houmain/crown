#pragma once

#include "platform/platform.h"
#include "Resource.h"

enum class TextureFormat {
  R8,
  RG8,
  RGB8,
  RGBA8,
};

class Texture {
public:
  Texture() = default;
  explicit Texture(std::string_view filename);
  Texture(TextureFormat format, int width, int height);
  GLuint texture_object() const { return m_texture_object; }
  TextureFormat format() const { return m_format; }
  int width() const { return m_width; }
  int height() const { return m_height; }
  void bind(int unit) const;
  void filter_linear();
  void upload(const void* data, size_t size);

private:
  void create(const void* data);

  Resource<GLuint> m_texture_object;
  TextureFormat m_format{ };
  int m_width{ };
  int m_height{ };
};
