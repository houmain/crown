#pragma once

#include "framework/Resource.h"
#include "framework/ImageFile.h"
#include "nanovg/nanovg.h"

extern NVGcontext* vg;

class NvgImage {
public:
  NvgImage() = default;

  explicit NvgImage(const Asset& asset, int flags = 0) {
    auto img = ImageFile(asset);
    m_width = img.width();
    m_height = img.height();
    m_image = {
      nvgCreateImageRGBA(vg, img.width(), img.height(),
        flags, static_cast<const unsigned char*>(img.data())),
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



