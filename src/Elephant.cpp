#include "Elephant.h"

olc::Sprite *Elephant::sprLeft = nullptr;
olc::Sprite *Elephant::sprRight = nullptr;

bool Elephant::loadData() {
  sprLeft = new olc::Sprite("assets/graphics/Elephant_left.png");
  sprRight = new olc::Sprite("assets/graphics/Elephant_right.png");

  return sprLeft != nullptr && sprRight != nullptr;
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
  if (direction == LEFT) {
    pge->DrawSprite(pos, sprLeft);
  } else {
    pge->DrawSprite(pos, sprRight);
  }
}

void Elephant::playCollisionSound() {
  snd = olc::SOUND::LoadAudioSample("assets/sound/animal_collision.wav");
  Obstacle::playCollisionSound();
}
