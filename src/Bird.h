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

private:
  static olc::Sprite *sprLeft, *sprRight;
};

#endif
