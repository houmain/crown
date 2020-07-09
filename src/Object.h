#pragma once

#include "Entity.h"

class World;

class Object {
public:
  explicit Object(EntityType entity_type);

  float x() const { return m_x; }
  float y() const { return m_y; }
  float width() const { return m_width; }
  float height() const { return m_height; }
  float velocity_x() const { return m_velocity_x; }
  float velocity_y() const { return m_velocity_y; }
  float get_x_at(float frame_pos) const { return m_x + m_velocity_x * frame_pos; }
  float get_y_at(float frame_pos) const { return m_y + m_velocity_y * frame_pos; }
  bool on_ground() const { return m_on_ground; }
  bool overlaps(const Object& other) const;

  void set_entity_handle(Handle entity_handle);
  Entity& get_entity();
  void set_position(float x, float y);
  void set_size(float width, float height);
  void set_interaction_radius(float radius);
  void update(const World& world);
  void apply_force(float x, float y);
  void interact(Object& other);

private:
  float m_x{ };
  float m_y{ };
  float m_width{ };
  float m_height{ };
  float m_velocity_x{ };
  float m_velocity_y{ };
  float m_square_interaction_radius{ };

  EntityType m_entity_type{ };
  Handle m_entity_handle = ~Handle{ };
  bool m_on_ground{ };
};
