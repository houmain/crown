
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

extern "C" {
#include "libxm/xm.h"
#include "libxm/src/xm_internal.h"
}

#include "framework/Texture.h"
#include "framework/SpriteBatch.h"
#include "framework/Target.h"
#include "framework/get_resource.h"
#include "framework/draw_tearing_indicator.h"

#include "Game.h"
#include <memory>

const auto native_width = 1680 / 2;
const auto native_height = 1050 / 2;

NVGcontext* vg;

const char* platform_window_title = "The Crown And The Bombs";
const int platform_minimum_width = native_width;
const int platform_minimum_height = native_height;
const int platform_initial_width = native_width * 2;
const int platform_initial_height = native_height * 2;

namespace {
  Target target;
  SpriteBatch batch;
} // namespace

void platform_on_setup() {
  vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

  const auto font = get_resource("upheavtt.ttf");
  nvgCreateFontMem(vg, "sans",
    static_cast<unsigned char*>(const_cast<void*>(font.first)),
    static_cast<int>(font.second), 0);

  bind_default_target(native_width, native_height);
  Game::instantiate();
}

void platform_music_callback(float* output_frames, int frame_count) {
#if defined(NDEBUG)
  static xm_context_t* xm_context;
  if (!xm_context) {
    auto xm = get_resource("dualtrax.xm");
    xm_create_context_safe(&xm_context,
      reinterpret_cast<const char*>(xm.first), xm.second, 44100);

    xm_context->global_volume = 0.5f;
  }
  xm_generate_samples(xm_context, output_frames, frame_count);
#endif
}

void platform_on_key(KeyCode key, bool down) {
  auto& player = game->player();
  switch (key) {
    case KeyCode::ArrowUp:
    case KeyCode::KeyX:
      player.apply_input(ActorInput::jump, down);
      break;

    case KeyCode::ControlLeft:
    case KeyCode::ControlRight:
    case KeyCode::KeyC:
      player.apply_input(ActorInput::attack, down);
      break;

    case KeyCode::ArrowLeft:
      player.apply_input(ActorInput::move_left, down);
      break;

    case KeyCode::ArrowRight:
      player.apply_input(ActorInput::move_right, down);
      break;

    case KeyCode::KeyR:
      if (down) {
        bind_default_target(native_width, native_height);
        Game::instantiate();
      }
      break;

    default:
      break;
  }
}

void platform_on_text(char code) {
}

void platform_on_mouse_button(int button, bool down) {
}

void platform_on_mouse_move(int x, int y) {
}

void platform_on_mouse_wheel(double dx, double dy) {
}

void platform_on_update(int window_width, int window_height, double time) {

#if defined(__WAJIC__)
  bind_default_target(window_width, window_height);

  game->update(time);
  game->draw();

#else
  const auto scale = std::min(
    window_width / static_cast<float>(native_width),
    window_height / static_cast<float>(native_height));
  const auto scale_int = std::max(static_cast<int>(scale), 1);

  // draw to target buffer
  target.bind(native_width * scale_int, native_height * scale_int,
    static_cast<float>(scale_int));

  // update and draw game
  game->update(time);
  game->draw();

  // scale target buffer to backbuffer
  bind_default_target(window_width, window_height);
  glClearColor(0.0, 0.0, 0.0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  const auto offset_x = (window_width - target.width() * scale / scale_int) / 2;
  const auto offset_y = (window_height - target.height() * scale / scale_int) / 2;
  target.filter_linear();
  batch.draw(offset_x, offset_y, target.texture(),
    0, 0, target.width(), target.height(), 1, scale / scale_int, false, true);
  batch.flush();
#endif

#if 0
  draw_tearing_indicator(window_width, window_height, time);
#endif
}
