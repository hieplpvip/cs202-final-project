#include "Bird.h"

olc::Sprite *Bird::sprLeft = nullptr;
olc::Sprite *Bird::sprRight = nullptr;
int Bird::sndCollision = -1;

Bird::Bird() {
  size = {(float)sprLeft->width, (float)sprLeft->height};
}

bool Bird::loadData() {
  sprLeft = new olc::Sprite("assets/graphics/Bird_fly_left.png");
  sprRight = new olc::Sprite("assets/graphics/Bird_fly_right.png");
  sndCollision = olc::SOUND::LoadAudioSample("assets/sound/animal_collision.wav");

  return sprLeft != nullptr && sprRight != nullptr && sndCollision != -1;
}

void Bird::unloadData() {
  if (sprLeft) {
    delete sprLeft;
    sprRight = nullptr;
  }
  if (sprRight) {
    delete sprRight;
    sprRight = nullptr;
  }
}

void Bird::draw() {
  pge->SetPixelMode(olc::Pixel::MASK);
  if (direction == LEFT) {
    pge->DrawSprite(pos, sprLeft);
  } else {
    pge->DrawSprite(pos, sprRight);
  }
  pge->SetPixelMode(olc::Pixel::NORMAL);
}

void Bird::playCollisionSound() {
  olc::SOUND::PlaySample(sndCollision);
}
