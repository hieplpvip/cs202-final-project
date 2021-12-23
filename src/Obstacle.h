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
  Obstacle();
  ~Obstacle();

  void setDirection(DIRECTION direction);
  void move(float fElapsedTime);
  void draw();
  void playCollisionSound();

protected:
  int snd;
  olc::Sprite* sprite = nullptr;
  DIRECTION direction;
};

#endif
