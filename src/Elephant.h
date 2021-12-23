#ifndef ELEPHANT_H
#define ELEPHANT_H

#include "Obstacle.h"

class Elephant : public Obstacle {
  void draw();
  void playCollisionSound();
};

#endif
