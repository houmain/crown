
#include "State.h"

void State::update() {
  m_timeline.step(update_interval());
}
