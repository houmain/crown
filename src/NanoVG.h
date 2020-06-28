#pragma once

#include "framework/Resource.h"
#include "framework/ImageFile.h"
#include "nanovg/nanovg.h"

extern NVGcontext* vg;

class NvgImage {
public:
  NvgImage() = default;

  explicit NvgImage(std::string_view filename, int flags = 0) {
    auto img = ImageFile(filename);
    m_width = img.width();
    m_height = img.height();
    m_image = {
      nvgCreateImageRGBA(vg, img.width(), img.height(),
        flags, (const unsigned char*)img.data()),
      [](int image) { nvgDeleteImage(vg, image); }
    };
  }

  int width() const { return m_width; }
  int height() const { return m_height; }
  operator int() const { return m_image; }

private:
  Resource<int> m_image{ };
  int m_width{ };
  int m_height{ };
};



