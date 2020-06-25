#pragma once

#include "platform/platform.h"

void draw_tearing_indicator(int width, int height, double time, int size = 10, double interval = 4) {
  auto pos = std::fmod(time, interval) / interval * 2;
  if (pos > 1)
    pos = 1 - (pos - 1);
  glEnable(GL_SCISSOR_TEST);
  glScissor(static_cast<int>(pos * (width - size)), 0, size, height);
  glClearColor(0, 1, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_SCISSOR_TEST);
}
