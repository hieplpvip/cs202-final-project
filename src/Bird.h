#ifndef BIRD_H
#define BIRD_H

#include "Obstacle.h"

class Bird : public Obstacle {
  void draw();
  void playCollisionSound();
};

#endif
