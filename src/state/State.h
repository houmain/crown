#pragma once

#include "tweakable.h"
#include "choreograph/Choreograph.h"

class State {
public:
  double update_interval() const { return 1 / 5.0; }
  const choreograph::Timeline& timeline() const { return m_timeline; }
  void update();

private:
  choreograph::Timeline m_timeline;
};
