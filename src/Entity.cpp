#include "Entity.h"

olc::vf2d Entity::getPosition() const {
  return pos;
}

void Entity::setPosition(olc::vf2d pos) {
  this->pos = pos;
}

olc::vf2d Entity::getSize() const {
  return size;
}

void Entity::setSize(olc::vf2d size) {
  this->size = size;
}

float Entity::getSpeed() const {
  return speed;
}

void Entity::setSpeed(float speed) {
  this->speed = speed;
}

bool Entity::intersectWithEntity(Entity* other) const {
  return (pos.x < other->pos.x + other->size.x) && (pos.x + size.x > other->pos.x) &&
         (pos.y < other->pos.y + other->size.y) && (pos.y + size.y > other->pos.y);
}
