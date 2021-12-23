#ifndef ELEPHANT_H
#define ELEPHANT_H

#include "Obstacle.h"

class Elephant : public Obstacle {
public:
  static bool loadData();
  static void unloadData();

  void draw();
  void playCollisionSound();

private:
  static olc::Sprite *sprLeft, *sprRight;
};

#endif
