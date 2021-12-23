#include "Elephant.h"

void Elephant::draw(olc::PixelGameEngine* pge) {
  if (direction == LEFT) {
    sprite = new olc::Sprite("assets/graphics/Elephant_left.png");
    // sprite = new olc::Sprite("assets/graphics/Elephant_walk_left.png");
  } else {
    sprite = new olc::Sprite("assets/graphics/Elephant_right.png");
    // sprite = new olc::Sprite("assets/graphics/Elephant_walk_left.png");
  }
  Obstacle::draw(pge);
}

void Elephant::playCollisionSound() {
  snd = olc::SOUND::LoadAudioSample("assets/sound/animal_collision.wav");
  Obstacle::playCollisionSound();
}
