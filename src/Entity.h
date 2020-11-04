#pragma once

#include <cstdint>
#include "tweakable.h"

using Handle = uint8_t;

enum class EntityType : uint8_t {
  player,
  pig,
  pig_king,
};

class Object;

class Entity {
public:
  virtual ~Entity() = default;
  Object& get_object();
  const Object& get_object() const;

  virtual void on_grounded() { }
  virtual void on_interaction(Entity& other) { }
  virtual void on_hit() { }

protected:
  Entity(EntityType type);

private:
  const EntityType m_type;
  const Handle m_object_handle;
};
