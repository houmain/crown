
#include "Object.h"
#include "World.h"
#include <cmath>

Object::Object(ObjectType type)
  : m_type(type) {
}

void Object::apply_force(float x, float y) {
  m_velocity_x += x;
  m_velocity_y += y;
}

void Object::set_position(float x, float y) {
  m_x = x;
  m_y = y;
}

void Object::set_size(float width, float height) {
  m_width = width;
  m_height = height;
}

void Object::update(const World& world) {
  // gravity
  m_velocity_y += TWEAKABLE(0.1f);

  // friction
  if (on_ground())
    m_velocity_x *= TWEAKABLE(0.85f);
  else
    m_velocity_x *= TWEAKABLE(0.95f);

  if (std::fabs(m_velocity_x) < TWEAKABLE(0.01))
    m_velocity_x = 0;

  // integration and collision with world
  const auto epsilon = 0.001f;
  const auto left = m_x - m_width / 2;
  const auto right = m_x + m_width / 2;
  const auto top = m_y - m_height;
  const auto bottom = m_y;

  const auto distance_to_wall_left = std::min(
    world.distance_to_wall_left(left, bottom),
    world.distance_to_wall_left(left, top));
  if (distance_to_wall_left <= -m_velocity_x) {
    m_x -= distance_to_wall_left - epsilon;
    m_velocity_x = 0;
  }
  else {
    const auto distance_to_wall_right = std::min(
      world.distance_to_wall_right(right, bottom),
      world.distance_to_wall_right(right, top));
    if (distance_to_wall_right <= m_velocity_x) {
      m_x += distance_to_wall_right - epsilon;
      m_velocity_x = 0;
    }
    else {
      m_x += m_velocity_x;
    }
  }

  const auto distance_to_ground = std::min(
    world.distance_to_ground(left, bottom),
    world.distance_to_ground(right, bottom));
  if (distance_to_ground <= m_velocity_y) {
    m_y += distance_to_ground - epsilon;
    m_velocity_y = 0;
    m_on_ground = true;
  }
  else {
    m_on_ground = false;

    const auto distance_to_ceiling = std::min(
      world.distance_to_ceiling(left, top),
      world.distance_to_ceiling(right, top));

    if (distance_to_ceiling <= -m_velocity_y) {
      m_y -= distance_to_ceiling - epsilon;
      m_velocity_y = 0;
    }
    else {
      m_y += m_velocity_y;
    }
  }
}
