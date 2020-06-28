
#include "Object.h"

void Object::update() {
  m_x += m_velocity_x;
  m_y += m_velocity_y;
}
