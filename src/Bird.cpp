#include "Bird.h"

olc::Sprite *Bird::sprLeft = nullptr;
olc::Sprite *Bird::sprRight = nullptr;

bool Bird::loadData() {
  sprLeft = new olc::Sprite("assets/graphics/Bird_fly_left.png");
  sprRight = new olc::Sprite("assets/graphics/Bird_fly_right.png");

  return sprLeft != nullptr && sprRight != nullptr;
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
  snd = olc::SOUND::LoadAudioSample("assets/sound/animal_collision.wav");
  Obstacle::playCollisionSound();
}
