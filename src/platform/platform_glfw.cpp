
#include "platform.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define MA_NO_DECODING
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include <cstdio>
#include <vector>
#include <atomic>

namespace {
  GLFWwindow* g_window;

  int find_first_zero(uint32_t bitmask) {
    for (auto i = 0; ; ++i, bitmask >>= 1)
      if ((bitmask & 0x01) == 0)
        return i;
  }

  class AudioBufferManager {
  public:
    static const auto max_buffers = 8;

    struct PlayingBuffer : AudioBuffer {
      int position;
      float volume_left;
      float volume_right;
    };

    void play(AudioBuffer&& buffer, float volume_left, float volume_right) {
      const auto index = find_first_zero(m_being_consumed.load());
      if (index < max_buffers) {
        m_buffers[index] = { std::move(buffer), 0, volume_left, volume_right };
        m_being_consumed.fetch_add(1 << index);
      }
    }

    template<typename F> // F(PlayingBuffer& buffer)
    void for_each_playing_buffer(F&& callback) {
      const auto being_consumed = m_being_consumed.load();
      for (auto i = 0; i < max_buffers; ++i)
        if (being_consumed & (1 << i)) {
          auto& buffer = m_buffers[i];
          callback(buffer);
          if (buffer.position >= buffer.sample_count)
            m_being_consumed.fetch_sub(1 << i);
        }
    }

  private:
    std::array<PlayingBuffer, max_buffers> m_buffers;
    std::atomic<uint32_t> m_being_consumed{ };
  };

  AudioBufferManager g_audio_buffer_manager;

  void set_fullscreen(bool fullscreen) {
    auto monitor = glfwGetPrimaryMonitor();
    const auto video_mode = glfwGetVideoMode(monitor);
    auto x = 0, y = 0, w = video_mode->width, h = video_mode->height;
    if (!fullscreen) {
      x += (w - platform_initial_width) / 2;
      y += (h - platform_initial_height) / 2;
      w = platform_initial_width;
      h = platform_initial_height;
      monitor = nullptr;
    }
    glfwSetWindowMonitor(g_window, monitor, x, y, w, h, video_mode->refreshRate);
    if (!fullscreen)
      glfwRestoreWindow(g_window);
  }

  void error_callback(int, const char* message) {
    platform_error(message);
  }

  void maximize_callback(GLFWwindow*, int maximized) {
    set_fullscreen(maximized == GLFW_TRUE);
  }

  void key_callback(GLFWwindow*, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      if (glfwGetWindowAttrib(g_window, GLFW_MAXIMIZED))
        set_fullscreen(false);

    platform_on_key(static_cast<KeyCode>(key), (action != GLFW_RELEASE));
  }

  void text_callback(GLFWwindow*, unsigned int code) {
    platform_on_text(code >= ' ' && code <= '~' ? static_cast<char>(code) : '?');
  }

  void mouse_button_callback(GLFWwindow*, int button, int action, int) {
    platform_on_mouse_button(button, action == GLFW_PRESS);
  }

  void mouse_move_callback(GLFWwindow*, double x, double y) {
    platform_on_mouse_move(static_cast<int>(x), static_cast<int>(y));
  }

  void mouse_wheel_callback(GLFWwindow*, double deltax, double deltay) {
    platform_on_mouse_wheel(deltax, deltay);
  }

  void audio_callback(ma_device*, void* output_frames_, const void*, ma_uint32 frame_count_) {
    const auto output_frames = static_cast<float*>(output_frames_);
    const auto frame_count = static_cast<int>(frame_count_);

    if (output_frames && frame_count) {
      platform_music_callback(output_frames, frame_count);

      g_audio_buffer_manager.for_each_playing_buffer(
        [&](AudioBufferManager::PlayingBuffer& buffer) {
          const auto sample_count =
            std::min(buffer.sample_count - buffer.position, frame_count);
          const auto volume_left = buffer.volume_left;
          const auto volume_right = buffer.volume_right;

          for (auto i = 0; i < 2 * sample_count; ) {
            const auto sample = (buffer.samples.get())[buffer.position++];
            output_frames[i++] += sample * volume_left;
            output_frames[i++] += sample * volume_right;
          }
        });
    }
  }
} // namespace

void platform_error(const char* message) {
  std::fprintf(stderr, "%s\n", message);
  std::abort();
}

void platform_play_audio(AudioBuffer buffer, float volume_left, float volume_right) {
  g_audio_buffer_manager.play(std::move(buffer), volume_left, volume_right);
}

#if !defined(_WIN32)
int main() {
#else
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    return EXIT_FAILURE;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  g_window = glfwCreateWindow(
    platform_initial_width, platform_initial_height,
    platform_window_title, NULL, NULL);
  if (!g_window) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  set_fullscreen(false);
  glfwSetWindowSizeLimits(g_window,
    platform_minimum_width, platform_minimum_height,
    GLFW_DONT_CARE, GLFW_DONT_CARE);
  glfwSetWindowAspectRatio(g_window,
    platform_minimum_width, platform_minimum_height);

  glfwSetWindowMaximizeCallback(g_window, maximize_callback);
  glfwSetKeyCallback(g_window, key_callback);
  glfwSetCharCallback(g_window, text_callback);
  glfwSetMouseButtonCallback(g_window, mouse_button_callback);
  glfwSetCursorPosCallback(g_window, mouse_move_callback);
  glfwSetScrollCallback(g_window, mouse_wheel_callback);

  glfwMakeContextCurrent(g_window);
#if defined(_WIN32)
  gladLoadGL(glfwGetProcAddress);
#endif
  glfwSwapInterval(1);

  auto audio_device_config = ma_device_config_init(ma_device_type_playback);
  audio_device_config.playback.format   = ma_format_f32;
  audio_device_config.playback.channels = 2;
  audio_device_config.sampleRate = 44100;
  audio_device_config.dataCallback = audio_callback;

  auto audio_device = ma_device{ };
  if (ma_device_init(nullptr, &audio_device_config, &audio_device) != MA_SUCCESS)
    return EXIT_FAILURE;

  if (ma_device_start(&audio_device) != MA_SUCCESS)
    return EXIT_FAILURE;

  platform_on_setup();

  while (!glfwWindowShouldClose(g_window)) {
    auto width = 0, height = 0;
    glfwGetWindowSize(g_window, &width, &height),
    platform_on_update(width, height, glfwGetTime());

    glfwSwapBuffers(g_window);
    glfwPollEvents();
  }

  ma_device_uninit(&audio_device);

  glfwDestroyWindow(g_window);

  glfwTerminate();
  return EXIT_SUCCESS;
}
