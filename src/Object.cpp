
#include "Object.h"
#include "Game.h"
#include <cmath>

Object::Object(ObjectType type, float x, float y)
  : m_type(type),
    m_x(x),
    m_y(y) {
}

void Object::apply_force(float x, float y) {
  m_velocity_x += x;
  m_velocity_y += y;
}

void Object::set_size(float width, float height) {
  m_width = width;
  m_height = height;
}

void Object::update() {  
  // gravity
  m_velocity_y += TWEAKABLE(0.1f);

  // friction
  if (on_ground())
    m_velocity_x *= TWEAKABLE(0.85f);
  else
    m_velocity_x *= TWEAKABLE(0.9f);

  if (std::fabs(m_velocity_x) < TWEAKABLE(0.05))
    m_velocity_x = 0;

  // integration and collision with world
  const auto epsilon = 0.001f;
  const auto left = m_x - m_width / 2;
  const auto right = m_x + m_width / 2;
  const auto top = m_y - m_height;
  const auto bottom = m_y;

  const auto distance_to_wall_left = std::min(
    game->distance_to_wall_left(left, bottom),
    game->distance_to_wall_left(left, top));
  if (distance_to_wall_left <= -m_velocity_x) {
    m_x -= distance_to_wall_left - epsilon;
    m_velocity_x = 0;
  }
  else {
    const auto distance_to_wall_right = std::min(
      game->distance_to_wall_right(right, bottom),
      game->distance_to_wall_right(right, top));
    if (distance_to_wall_right <= m_velocity_x) {
      m_x += distance_to_wall_right - epsilon;
      m_velocity_x = 0;
    }
    else {
      m_x += m_velocity_x;
    }
  }

  const auto distance_to_ground = std::min(
    game->distance_to_ground(left, bottom),
    game->distance_to_ground(right, bottom));
  if (distance_to_ground <= m_velocity_y) {
    m_y += distance_to_ground - epsilon;
    m_velocity_y = 0;
    m_on_ground = true;
  }
  else {
    m_on_ground = false;

    const auto distance_to_ceiling = std::min(
      game->distance_to_ceiling(left, top),
      game->distance_to_ceiling(right, top));

    if (distance_to_ceiling <= -m_velocity_y) {
      m_y -= distance_to_ceiling - epsilon;
      m_velocity_y = 0;
    }
    else {
      m_y += m_velocity_y;
    }
  }
}
