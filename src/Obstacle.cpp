#include "Obstacle.h"

Obstacle::Obstacle() {
  pos = {0, 0};
  size = {20, 20};
}

Obstacle::~Obstacle() {
  if (sprite) {
    delete sprite;
  }
}

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

void Obstacle::draw(olc::PixelGameEngine* pge) {
  if (sprite) {
    pge->DrawSprite(pos, sprite);
  }
}

void Obstacle::playCollisionSound() {
  olc::SOUND::PlaySample(snd);
}
