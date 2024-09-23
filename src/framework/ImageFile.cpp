
#include "ImageFile.h"
#include "stb/stb_image.h"

ImageFile::ImageFile(const Asset& asset, int components) {
  m_data = {
    stbi_load_from_memory(
      reinterpret_cast<const stbi_uc*>(asset.data), 
      static_cast<int>(asset.size),
      &m_width, &m_height, &m_components, components),
    [](void* handle) { stbi_image_free(handle); }
  };
}
