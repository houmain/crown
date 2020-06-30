
#include "Game.h"

Game* game;

Game::Game()
  : m_player((m_objects.emplace_back(ObjectType::player, 380, 90), &m_objects.back())) {

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  const auto cells_x = target_width() / tile_size;
  const auto cells_y = target_height() / tile_size;
  generate_world(cells_x, cells_y, 5);
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

  m_player.update();

  for (auto& object : m_objects)
    if (object) {
      object.update();
    }
}

void Game::draw() {
  m_draw_timeline.jumpTo(m_update_timeline.time() + m_update_time * update_interval());
  const auto frame_pos = static_cast<float>(m_update_time);

  draw_world();

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  draw_animation(32 * 3.5f, 32 * 8, sprites::king_pig_idle,
    6 * static_cast<float>(m_draw_timeline.time()));

  static float frame = 0;
  frame += m_player.object().velocity_x() * TWEAKABLE(0.15);

  if (m_player.object().velocity_x()) {
    draw_animation(
      m_player.object().get_x_at(frame_pos),
      m_player.object().get_y_at(frame_pos),
      sprites::player_run, frame,
      m_player.looking_left());
  }
  else {
    draw_animation(
      m_player.object().get_x_at(frame_pos),
      m_player.object().get_y_at(frame_pos),
      sprites::player_idle, m_draw_timeline.time() * 6,
      m_player.looking_left());
  }

  flush_drawing();
}
