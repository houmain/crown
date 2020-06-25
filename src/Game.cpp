
#include "Game.h"

void Game::update(double time) {
  const auto time_elapsed = (time - m_previous_time);
  m_previous_time = time;

  m_update_time += time_elapsed / m_state.update_interval();
  while (m_update_time > 1.0) {
    m_state.update();
    m_update_time -= 1.0;
  }

  m_view.draw(static_cast<float>(m_update_time));
}
