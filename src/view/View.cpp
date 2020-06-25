
#include "View.h"
#include "state/State.h"
#include <cmath>

View::View(const State* state)
  : m_state(*state),
    m_sprites("sprites.png") {

  m_timeline.apply(&m_radius)
    .then<choreograph::RampTo>(13, 1.5);

  auto target = Target();
  target.bind(target_width(), target_height());

  for (auto i = 0, y = 0; y < target_height() + 31; y += 32)
    for (auto x = 0; x < target_width() + 31; x += 32)
      draw_animation(x, y, sprites::world_tiles, i++);
  m_batch.flush();

  m_world = std::move(target);
}

void View::draw(float frame_pos) {
  m_timeline.jumpTo(m_state.timeline().time() + frame_pos * m_state.update_interval());

  m_batch.draw(0, 0, m_world, 0, 0, m_world.width(), m_world.height(), 1, 1, false, true);
  m_batch.flush();

  auto x = TWEAKABLE(10);
  auto y = TWEAKABLE(20);
  auto w = 100;
  auto h = 100;
  auto radius = m_radius.value();
  auto text = TWEAKABLE("hallo das ist ein Test99");
  auto color = TWEAKABLE(nvgRGBA, 10, 128, 255, 255);

  nvgBeginFrame(vg, target_width(), target_height(), target_scale());

  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 0.5f,y + 0.5f, w - 1, h - 1, radius - 0.25f);
  nvgStrokeColor(vg, color);
  nvgStroke(vg);

  nvgFontBlur(vg, TWEAKABLE(0));
  nvgFillColor(vg, nvgRGBA(255, 32, 64, 255));

  nvgFontSize(vg, TWEAKABLE(20));
  nvgText(vg, x + 20 * std::fabs(std::sin(m_timeline.time())), y - 5, text, nullptr);

  nvgEndFrame(vg);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  draw_animation(300, 200, sprites::king_pig_idle, 6 * static_cast<float>(m_timeline.time()));
  draw_animation(100+std::fmod(m_timeline.time(), 6) * 70, target_height(), sprites::player_run, 13 * static_cast<float>(m_timeline.time()));

  m_batch.flush();
}

void View::draw_sprite(float x, float y, const sprites::Sprite& sprite) {
  m_batch.draw(x - sprite.pivot_x, y - sprite.pivot_y, m_sprites,
    sprite.x, sprite.y, sprite.w, sprite.h);
}

void View::do_draw_animation(float x, float y,
    const sprites::Sprite* const* frames, float position) {
  const auto& frame = *frames[static_cast<int>(position)];
  draw_sprite(x, y, frame);
}
