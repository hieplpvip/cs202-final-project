#ifndef TRUCK_H
#define TRUCK_H

#include "Obstacle.h"

class Truck : public Obstacle {
public:
  Truck();

  static bool loadData();
  static void unloadData();

  void draw();
  void playCollisionSound();

private:
  static olc::Sprite *sprLeft, *sprRight;
  static int sndCollision;
};

#endif
