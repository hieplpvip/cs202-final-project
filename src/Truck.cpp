#include "Truck.h"

void Truck::draw() {
  if (direction == LEFT) {
    sprite = new olc::Sprite("assets/graphics/Truck_left.png");
  } else {
    sprite = new olc::Sprite("assets/graphics/Truck_right.png");
  }
  Obstacle::draw();
}

void Truck::playCollisionSound() {
  snd = olc::SOUND::LoadAudioSample("assets/sound/vehicle_collision.wav");
  Obstacle::playCollisionSound();
};
