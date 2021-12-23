#ifndef CAR_H
#define CAR_H

#include "Obstacle.h"

class Car : public Obstacle {
public:
  static bool loadData();
  static void unloadData();

  void draw();
  void playCollisionSound();

private:
  static olc::Sprite *sprLeft, *sprRight;
};

#endif
