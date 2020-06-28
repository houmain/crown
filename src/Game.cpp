
#include "Game.h"

Game::Game()
  : m_player(&m_objects.emplace_back()) {

  const auto cells_x = target_width() / tile_size;
  const auto cells_y = target_height() / tile_size;
  m_world.generate(cells_x, cells_y, 5);
  m_world_texture = m_world.generate_texture(*this);
}

void Game::update(double time) {
  const auto time_elapsed = (time - m_previous_time);
  m_previous_time = time;

  m_update_time += time_elapsed / update_interval();
  while (m_update_time > 1.0) {
    do_update();
    m_update_time -= 1.0;
  }
}

void Game::do_update() {
  m_update_timeline.step(update_interval());
}

void Game::draw() {
  m_draw_timeline.jumpTo(m_update_timeline.time() + m_update_time * update_interval());
  const auto time = m_draw_timeline.time();

  draw_texture(0, 0, m_world_texture, 0, 0,
    m_world_texture.width(), m_world_texture.height(), 1, 1, false, true);
  flush_drawing();

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  draw_animation(300, 200, sprites::king_pig_idle, 6 * static_cast<float>(time));
  draw_animation(100 + std::fmod(time, 6) * 70, target_height(), sprites::player_run,
    13 * static_cast<float>(time));
  flush_drawing();
}
