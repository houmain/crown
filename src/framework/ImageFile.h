#pragma once

#include "Resource.h"
#include "Asset.h"
#include <string>

class ImageFile {
public:
  ImageFile() = default;
  explicit ImageFile(const Asset& asset, int components = 0);
  int width() const { return m_width; }
  int height() const { return m_height; }
  int components() const { return m_components; }
  const void* data() const { return m_data; }

private:
  Resource<void*> m_data;
  int m_width{ };
  int m_height{ };
  int m_components{ };
};
