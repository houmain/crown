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

extern const char* platform_window_title;
extern const int platform_initial_width;
extern const int platform_initial_height;
extern const int platform_minimum_width;
extern const int platform_minimum_height;

void platform_on_setup();
void platform_on_update(int width, int height, double time);
void platform_on_key(int code, bool down);
void platform_on_text(char code);
void platform_on_mouse_button(int button, bool down);
void platform_on_mouse_move(int x, int y);
void platform_on_mouse_wheel(double dx, double dy);
void platform_music_callback(float* output_frames, int frame_count);

void platform_error(const char* message);
