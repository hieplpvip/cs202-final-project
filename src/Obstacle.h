#ifndef OBSTACLE_H
#define OBSTACLE_H

// clang-format off
#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"
// clang-format on

#include "Entity.h"

enum DIRECTION {
  LEFT,
  RIGHT,
};

class Obstacle : public Entity {
public:
  void setDirection(DIRECTION direction);
  void move(float fElapsedTime);
  virtual void playCollisionSound() = 0;

protected:
  DIRECTION direction;
};

#endif
