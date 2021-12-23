#include "Truck.h"

olc::Sprite *Truck::sprLeft = nullptr;
olc::Sprite *Truck::sprRight = nullptr;

bool Truck::loadData() {
  sprLeft = new olc::Sprite("assets/graphics/Truck_left.png");
  sprRight = new olc::Sprite("assets/graphics/Truck_right.png");

  return sprLeft != nullptr && sprRight != nullptr;
}

void Truck::unloadData() {
  if (sprLeft) {
    delete sprLeft;
    sprRight = nullptr;
  }
  if (sprRight) {
    delete sprRight;
    sprRight = nullptr;
  }
}

void Truck::draw() {
  if (direction == LEFT) {
    pge->DrawSprite(pos, sprLeft);
  } else {
    pge->DrawSprite(pos, sprRight);
  }
}

void Truck::playCollisionSound() {
  snd = olc::SOUND::LoadAudioSample("assets/sound/vehicle_collision.wav");
  Obstacle::playCollisionSound();
};
