#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Entity.h"

enum DIRECTION {
  LEFT,
  RIGHT,
};

class Obstacle : public Entity {
public:
	Obstacle();
  void setDirection(DIRECTION direction);
  void move(float fElapsedTime);
  void draw(olc::PixelGameEngine* pge);
  ~Obstacle() {
	  if (sprite)
		  delete sprite;
  }
  void playCollisionSound();

protected:
  olc::Sprite* sprite = nullptr;
  DIRECTION direction;
};

#endif
