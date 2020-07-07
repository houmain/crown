
#include "Object.h"
#include "World.h"
#include "Game.h"
#include <cmath>

Object::Object(EntityType entity_type)
  : m_entity_type(entity_type) {
}

void Object::set_entity_handle(Handle entity_handle) {
  m_entity_handle = entity_handle;
}

Entity& Object::get_entity() {
  return game->get_entity(m_entity_type, m_entity_handle);
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
  m_velocity_y += TWEAKABLE(0.2f);

  // friction
  if (on_ground())
    m_velocity_x *= TWEAKABLE(0.9f);
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
  if (distance_to_wall_left < -m_velocity_x + epsilon) {
    m_x -= distance_to_wall_left - epsilon;
    m_velocity_x = 0;
  }
  else {
    const auto distance_to_wall_right = std::min(
      world.distance_to_wall_right(right, bottom),
      world.distance_to_wall_right(right, top));
    if (distance_to_wall_right < m_velocity_x - epsilon) {
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
  if (distance_to_ground < m_velocity_y + epsilon) {
    m_y += distance_to_ground - epsilon;

    if (!m_on_ground)
      get_entity().on_grounded();

    m_velocity_y = 0;
    m_on_ground = true;
  }
  else {
    m_on_ground = false;

    const auto distance_to_ceiling = std::min(
      world.distance_to_ceiling(left, top),
      world.distance_to_ceiling(right, top));

    if (distance_to_ceiling < -m_velocity_y - epsilon) {
      m_y -= distance_to_ceiling - epsilon;
      m_velocity_y = 0;
    }
    else {
      m_y += m_velocity_y;
    }
  }
}

bool Object::overlaps(const Object& other) const {
  return !(m_x + m_width < other.m_x ||
           m_y + m_height < other.m_y ||
           other.m_x + other.m_width < m_x ||
           other.m_y + other.m_height < m_y);
}

void Object::interact(Object &other) {
  if (overlaps(other)) {
    // TODO
  }
}
