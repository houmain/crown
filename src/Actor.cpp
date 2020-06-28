
#include "Actor.h"
#include <cstdio>

Actor::Actor(Object* object)
  : m_object(*object) {
}

void Actor::apply_input(ActorInput input, bool down) {

}

void Actor::update() {
}
