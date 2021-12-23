#include "Car.h"

olc::Sprite *Car::sprLeft = nullptr;
olc::Sprite *Car::sprRight = nullptr;

bool Car::loadData() {
  sprLeft = new olc::Sprite("assets/graphics/Car_left.png");
  sprRight = new olc::Sprite("assets/graphics/Car_right.png");

  return sprLeft != nullptr && sprRight != nullptr;
}

void Car::unloadData() {
  if (sprLeft) {
    delete sprLeft;
    sprRight = nullptr;
  }
  if (sprRight) {
    delete sprRight;
    sprRight = nullptr;
  }
}

void Car::draw() {
  if (direction == LEFT) {
    pge->DrawSprite(pos, sprLeft);
  } else {
    pge->DrawSprite(pos, sprRight);
  }
}

void Car::playCollisionSound() {
  snd = olc::SOUND::LoadAudioSample("assets/sound/vehicle_collision.wav");
  Obstacle::playCollisionSound();
};
