#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
public:
  Player();

  void moveUp(float fElapsedTime);
  void moveDown(float fElapsedTime);
  void moveLeft(float fElapsedTime);
  void moveRight(float fElapsedTime);
  void init();
  int getX();
  int getY();
  void setX(int x);
  void setY(int y);

  void draw(olc::PixelGameEngine* pge);
};

#endif
