#include "Obstacle.h"

void Obstacle::setDirection(DIRECTION direction) {
  this->direction = direction;
}

void Obstacle::move(float fElapsedTime) {
  switch (direction) {
    case RIGHT:
      pos.x += fElapsedTime * speed;
      break;
    case LEFT:
      pos.x -= fElapsedTime * speed;
      break;
  }
}

void Obstacle::playCollisionSound() {
  olc::SOUND::PlaySample(snd);
}
