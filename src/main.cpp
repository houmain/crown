
#include "platform/platform.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_NO_FAILURE_STRINGS
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

#define NANOVG_GLES2_IMPLEMENTATION
#if !defined(GL_LUMINANCE)
# define GL_LUMINANCE 0x1909
#endif
#if defined(GL_GENERATE_MIPMAP)
# undef GL_GENERATE_MIPMAP
#endif
#define GL_GENERATE_MIPMAP GL_AUTO_GENERATE_MIPMAP
#include "nanovg/nanovg.h"
#include "nanovg/nanovg_gl.h"
#include "nanovg/nanovg_gl_utils.h"

#define TWEAKABLE_IMPLEMENTATION
#include "tweakable.h"

#include "framework/Texture.h"
#include "framework/SpriteBatch.h"
#include "framework/Target.h"
#include "framework/get_resource.h"
#include "framework/draw_tearing_indicator.h"

#include "Game.h"
#include <memory>

const auto native_width = 1920 / 3;
const auto native_height = 1080 / 3;

NVGcontext* vg;

const char* platform_window_title = "The Crown And The Bombs";
const int platform_minimum_width = native_width;
const int platform_minimum_height = native_height;
const int platform_initial_width = native_width * 2;
const int platform_initial_height = native_height * 2;

namespace {
  Target target;
  SpriteBatch batch;
  std::unique_ptr<Game> game;
} // namespace

void platform_on_setup() {
  vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

  const auto font = get_resource("upheavtt.ttf");
  nvgCreateFontMem(vg, "sans",
    static_cast<unsigned char*>(const_cast<void*>(font.first)),
    font.second, 0);

  bind_default_target(native_width, native_height);
  game = std::make_unique<Game>();
}

void platform_on_key(int code, bool down) {
}

void platform_on_text(char code) {
}

void platform_on_mouse_button(int button, bool down) {
}

void platform_on_mouse_move(int x, int y) {
}

void platform_on_mouse_wheel(float dx, float dy) {
}

void platform_on_update(int window_width, int window_height, double time) {

#if defined(__WAJIC__)
  bind_default_target(window_width, window_height);

  game->update(time);

#else
  const auto scale = std::min(
    window_width / static_cast<float>(native_width),
    window_height / static_cast<float>(native_height));
  const auto scale_int = std::max(1.0f, std::floor(scale));

  // draw to target buffer
  target.bind(native_width * scale_int, native_height * scale_int, scale_int);

  // update and draw game
  game->update(time);

  // scale target buffer to backbuffer
  bind_default_target(window_width, window_height);
  glClearColor(0.0, 0.0, 0.0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_BLEND);

  const auto offset_x = (window_width - target.width() * scale / scale_int) / 2;
  const auto offset_y = (window_height - target.height() * scale / scale_int) / 2;
  target.filter_linear();
  batch.draw(offset_x, offset_y, target.texture(),
    0, 0, target.width(), target.height(), 1, scale / scale_int, false, true);
  batch.flush();
#endif

#if 1
  draw_tearing_indicator(window_width, window_height, time);
#endif
}
