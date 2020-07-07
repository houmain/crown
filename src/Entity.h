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
  Object& get_object();

  virtual void on_grounded() { }

protected:
  Entity(EntityType type);

private:
  const EntityType m_type;
  const Handle m_object_handle;
};
