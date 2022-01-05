#ifndef CAR_H
#define CAR_H

#include "Obstacle.h"

class Car : public Obstacle {
public:
  Car();

  static bool loadData();
  static void unloadData();

  void draw();
  void playCollisionSound();
  TYPE getType();

private:
  static olc::Sprite *sprLeft, *sprRight;
  static int sndCollision;
};

#endif
