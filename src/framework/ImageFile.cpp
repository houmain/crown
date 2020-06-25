
#include "ImageFile.h"
#include "stb/stb_image.h"
#include "get_resource.h"

ImageFile::ImageFile(std::string_view filename, int components) {
  auto res = get_resource(filename);
  m_data = {
    stbi_load_from_memory((const stbi_uc*)res.first, res.second,
      &m_width, &m_height, &m_components, components),
    [](void* handle) { stbi_image_free(handle); }
  };
}
