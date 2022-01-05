#include "Truck.h"

olc::Sprite *Truck::sprLeft = nullptr;
olc::Sprite *Truck::sprRight = nullptr;
int Truck::sndCollision = -1;

Truck::Truck() {
  size = {(float)sprLeft->width, (float)sprLeft->height};
}

bool Truck::loadData() {
  sprLeft = new olc::Sprite("assets/graphics/Truck_left.png");
  sprRight = new olc::Sprite("assets/graphics/Truck_right.png");
  sndCollision = olc::SOUND::LoadAudioSample("assets/sound/vehicle_collision.wav");

  return sprLeft != nullptr && sprRight != nullptr && sndCollision != -1;
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
  pge->SetPixelMode(olc::Pixel::MASK);
  if (direction == LEFT) {
    pge->DrawSprite(pos, sprLeft);
  } else {
    pge->DrawSprite(pos, sprRight);
  }
  pge->SetPixelMode(olc::Pixel::NORMAL);
}

void Truck::playCollisionSound() {
  olc::SOUND::PlaySample(sndCollision);
};

TYPE Truck::getType() {
  return VEHICLE;
}
