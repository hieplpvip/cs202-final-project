#include "Car.h"

void Car::draw() {
  if (direction == LEFT) {
    sprite = new olc::Sprite("assets/graphics/Car_left.png");
  } else {
    sprite = new olc::Sprite("assets/graphics/Car_right.png");
  }
  Obstacle::draw();
}

void Car::playCollisionSound() {
  snd = olc::SOUND::LoadAudioSample("assets/sound/vehicle_collision.wav");
  Obstacle::playCollisionSound();
};
