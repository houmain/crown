
#include "Game.h"

Game* game;

void Game::instantiate() {
  // first allocate and set pointer to instance before constructing
  game = static_cast<Game*>(::operator new(sizeof(Game)));
  new (game) Game();
}

Game::Game() {
  const auto cells_x = target_width() / m_world.tile_size;
  const auto cells_y = target_height() / m_world.tile_size;
  m_world.generate(cells_x, cells_y, 5);
  m_world.generate_texture(m_graphics, target_width(), target_height());

  m_player.get_object().set_position(380, 90);

  auto pig = m_pigs.emplace();
  m_pigs[pig].get_object().set_position(300, 80);

  pig = m_pigs.emplace();
  m_pigs[pig].get_object().set_position(270, 90);

  pig = m_pigs.emplace();
  m_pigs[pig].get_object().set_position(240, 20);
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

  for (auto& pig : m_pigs)
    pig.update();

  for (auto& object : m_objects)
    object.update(m_world);
}

Handle Game::allocate_object(ObjectType object_type) {
  return m_objects.emplace(object_type);
}

void Game::draw() {
  m_draw_timeline.jumpTo(m_update_timeline.time() + m_update_time * update_interval());
  const auto frame_pos = static_cast<float>(m_update_time);

  m_graphics.disable_blending();
  m_world.draw(m_graphics);

  m_graphics.enable_blending();
  for (auto& pig : m_pigs)
    pig.draw(m_graphics, frame_pos);

  m_player.draw(m_graphics, frame_pos);

  m_graphics.disable_blending();
}
