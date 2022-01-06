#include "Car.h"

olc::Sprite *Car::sprLeft = nullptr;
olc::Sprite *Car::sprRight = nullptr;
int Car::sndCollision = -1;

Car::Car() {
  size = {(float)sprLeft->width, (float)sprLeft->height};
}

bool Car::loadData() {
  sprLeft = new olc::Sprite("assets/graphics/Car_left.png");
  sprRight = new olc::Sprite("assets/graphics/Car_right.png");
  sndCollision = olc::SOUND::LoadAudioSample("assets/sound/vehicle_collision.wav");

  return sprLeft != nullptr && sprRight != nullptr && sndCollision != -1;
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
  pge->SetPixelMode(olc::Pixel::MASK);
  if (direction == LEFT) {
    pge->DrawSprite(pos, sprLeft);
  } else {
    pge->DrawSprite(pos, sprRight);
  }
  pge->SetPixelMode(olc::Pixel::NORMAL);
}

void Car::playCollisionSound() {
  olc::SOUND::PlaySample(sndCollision);
};
