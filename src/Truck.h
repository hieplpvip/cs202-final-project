#ifndef TRUCK_H
#define TRUCK_H

#include "Obstacle.h"

class Truck : public Obstacle {
  void draw();
  void playCollisionSound();
};

#endif
