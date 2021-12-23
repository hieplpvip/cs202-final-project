#ifndef CAR_H
#define CAR_H

#include "Obstacle.h"

class Car : public Obstacle {
  void draw();
  void playCollisionSound();
};

#endif
