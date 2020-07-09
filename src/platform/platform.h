#pragma once

#if defined(__WAJIC__)
# include <wajic_gl.h>
#elif defined(_WIN32)
# define NOMINMAX
# include <glad/gl.h>
#else
# define GL_GLEXT_PROTOTYPES
# include <GL/glcorearb.h>
#endif

#include <cmath>
#include <string>
#include <memory>

extern const char* platform_window_title;
extern const int platform_initial_width;
extern const int platform_initial_height;
extern const int platform_minimum_width;
extern const int platform_minimum_height;

enum class KeyCode {
  KeyA = 65, // GLFW_KEY_A
  KeyB = 66, // GLFW_KEY_B
  KeyC = 67, // GLFW_KEY_C
  KeyD = 68, // GLFW_KEY_D
  KeyE = 69, // GLFW_KEY_E
  KeyF = 70, // GLFW_KEY_F
  KeyG = 71, // GLFW_KEY_G
  KeyH = 72, // GLFW_KEY_H
  KeyI = 73, // GLFW_KEY_I
  KeyJ = 74, // GLFW_KEY_J
  KeyK = 75, // GLFW_KEY_K
  KeyL = 76, // GLFW_KEY_L
  KeyM = 77, // GLFW_KEY_M
  KeyN = 78, // GLFW_KEY_N
  KeyO = 79, // GLFW_KEY_O
  KeyP = 80, // GLFW_KEY_P
  KeyQ = 81, // GLFW_KEY_Q
  KeyR = 82, // GLFW_KEY_R
  KeyS = 83, // GLFW_KEY_S
  KeyT = 84, // GLFW_KEY_T
  KeyU = 85, // GLFW_KEY_U
  KeyV = 86, // GLFW_KEY_V
  KeyW = 87, // GLFW_KEY_W
  KeyX = 88, // GLFW_KEY_X
  KeyY = 89, // GLFW_KEY_Y
  KeyZ = 90, // GLFW_KEY_Z
  ArrowRight = 262, // GLFW_KEY_RIGHT
  ArrowLeft = 263, // GLFW_KEY_LEFT
  ArrowDown = 264, // GLFW_KEY_DOWN
  ArrowUp = 265, // GLFW_KEY_UP
};

struct AudioBuffer {
  std::shared_ptr<const float> samples;
  int sample_count;
};

void platform_on_setup();
void platform_on_update(int width, int height, double time);
void platform_on_key(KeyCode key, bool down);
void platform_on_text(char code);
void platform_on_mouse_button(int button, bool down);
void platform_on_mouse_move(int x, int y);
void platform_on_mouse_wheel(double dx, double dy);
void platform_music_callback(float* output_frames, int frame_count);

void platform_play_audio(AudioBuffer buffer);
void platform_error(const char* message);
