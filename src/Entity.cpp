
#include "Entity.h"
#include "Game.h"

Entity::Entity(EntityType entity_type)
  : m_type(entity_type),
    m_object_handle(game->allocate_object(entity_type)) {
}

Object& Entity::get_object() {
  return game->get_object(m_object_handle);
}

const Object& Entity::get_object() const {
  return game->get_object(m_object_handle);
}
