
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

void Object::set_interaction_radius(float radius) {
  m_square_interaction_radius = (radius * radius);
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
  const auto half_width = m_width / 2;

  const auto distance_to_wall_left = std::min(
    world.distance_to_wall_left(m_x - half_width, m_y),
    world.distance_to_wall_left(m_x - half_width, m_y - m_height));
  if (distance_to_wall_left < -m_velocity_x + epsilon) {
    m_x -= distance_to_wall_left - epsilon;
    m_velocity_x = 0;
  }
  else {
    const auto distance_to_wall_right = std::min(
      world.distance_to_wall_right(m_x + half_width, m_y),
      world.distance_to_wall_right(m_x + half_width, m_y - m_height));
    if (distance_to_wall_right < m_velocity_x - epsilon) {
      m_x += distance_to_wall_right - epsilon;
      m_velocity_x = 0;
    }
    else {
      m_x += m_velocity_x;
    }
  }

  const auto distance_to_ground = std::min(
    world.distance_to_ground(m_x - half_width, m_y),
    world.distance_to_ground(m_x + half_width, m_y));
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
      world.distance_to_ceiling(m_x - half_width, m_y - m_height),
      world.distance_to_ceiling(m_x + half_width, m_y - m_height));

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
  return !(m_x + m_width <= other.m_x ||
           m_y + m_height <= other.m_y ||
           other.m_x + other.m_width <= m_x ||
           other.m_y + other.m_height <= m_y);
}

void Object::interact(Object &other) {
  const auto dx = m_x - other.m_x;
  const auto dy = m_y - other.m_y;
  const auto square_distance = (dx * dx + dy * dy);
  if (square_distance < m_square_interaction_radius ||
      square_distance < other.m_square_interaction_radius) {

    auto& entity = get_entity();
    auto& other_entity = other.get_entity();

    if (square_distance < m_square_interaction_radius)
      entity.on_interaction(other_entity);

    if (square_distance < other.m_square_interaction_radius)
      other_entity.on_interaction(entity);
  }
}
