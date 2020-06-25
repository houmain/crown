#pragma once

#include "state/State.h"
#include "view/View.h"

class Game {
public:
  void update(double time);

private:
  State m_state;
  View m_view{ &m_state };

  double m_previous_time{ };
  double m_update_time{ };
};
