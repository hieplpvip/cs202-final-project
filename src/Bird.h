#ifndef BIRD_H
#define BIRD_H

#include "Obstacle.h"

class Bird : public Obstacle {
public:
  Bird();

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
