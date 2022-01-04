#include "Elephant.h"

olc::Sprite *Elephant::sprLeft = nullptr;
olc::Sprite *Elephant::sprRight = nullptr;
int Elephant::sndCollision = -1;

Elephant::Elephant() {
  size = {(float)sprLeft->width, (float)sprLeft->height};
}

bool Elephant::loadData() {
  sprLeft = new olc::Sprite("assets/graphics/Elephant_left.png");
  sprRight = new olc::Sprite("assets/graphics/Elephant_right.png");
  sndCollision = olc::SOUND::LoadAudioSample("assets/sound/animal_collision.wav");

  return sprLeft != nullptr && sprRight != nullptr && sndCollision != -1;
}

void Elephant::unloadData() {
  if (sprLeft) {
    delete sprLeft;
    sprRight = nullptr;
  }
  if (sprRight) {
    delete sprRight;
    sprRight = nullptr;
  }
}

void Elephant::draw() {
  pge->SetPixelMode(olc::Pixel::MASK);
  if (direction == LEFT) {
    pge->DrawSprite(pos, sprLeft);
  } else {
    pge->DrawSprite(pos, sprRight);
  }
  pge->SetPixelMode(olc::Pixel::NORMAL);
}

void Elephant::playCollisionSound() {
  olc::SOUND::PlaySample(sndCollision);
}
