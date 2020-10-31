
#include "Game.h"

Game* game;

void Game::instantiate() {
  // first allocate and set pointer to instance before constructing
  if (game) {
    game->~Game();
  }
  else {
    game = static_cast<Game*>(::operator new(sizeof(Game)));
  }
  new (game) Game();
}

Game::Game() {
  const auto cells_x = target_width() / m_world.tile_size;
  const auto cells_y = target_height() / m_world.tile_size;

  static auto seed = 1;
  m_world.generate(cells_x, cells_y, seed++);
  m_world.generate_texture(m_graphics, target_width(), target_height());

  m_player.get_object().set_position(380, 90);
  m_pig_king.get_object().set_position(300, 80);

  create_pig(270, 90);
  create_pig(240, 20);
}

void Game::update(double time) {
  const auto time_elapsed = (time - m_previous_time);
  if (m_previous_time) {
    m_update_time += time_elapsed / update_interval();
    while (m_update_time > 1.0) {
      do_update();
      m_update_time -= 1.0;
    }
  }
  m_previous_time = time;
}

void Game::do_update() {
  m_update_timeline.step(update_interval());

  m_player.update();
  m_pig_king.update();

  for (auto& pig : m_pigs)
    pig.update();

  for (auto& object : m_objects)
    object.update(m_world);

  for (auto it = m_objects.begin(), end = m_objects.end(); it != end; ++it)
    for (auto it2 = std::next(it); it2 != end; ++it2)
      it->interact(*it2);
}

Handle Game::allocate_object(EntityType entity_type) {
  return m_objects.emplace(entity_type);
}

Entity& Game::get_entity(EntityType entity_type, Handle handle) {
  switch (entity_type) {
    case EntityType::player: return m_player;
    case EntityType::pig: return m_pigs[handle];
    case EntityType::pig_king: return m_pig_king;
  }
  std::abort();
}

Pig& Game::create_pig(float x, float y) {
  auto handle = m_pigs.emplace();
  auto& pig = m_pigs[handle];
  auto& object = pig.get_object();
  object.set_entity_handle(handle);
  object.set_position(x, y);
  return pig;
}

void Game::draw() {
  m_draw_timeline.jumpTo(m_update_timeline.time() + m_update_time * update_interval());
  const auto frame_pos = static_cast<float>(m_update_time);

  m_graphics.disable_blending();
  m_world.draw(m_graphics);

  m_graphics.enable_blending();

  for (auto& pig : m_pigs)
    pig.draw(m_graphics, frame_pos);

  m_pig_king.draw(m_graphics, frame_pos);
  m_player.draw(m_graphics, frame_pos);

  m_graphics.disable_blending();
}
