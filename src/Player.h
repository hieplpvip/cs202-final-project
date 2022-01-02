#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
public:
  Player();

  static bool loadData();
  static void unloadData();

  void moveUp(float fElapsedTime);
  void moveDown(float fElapsedTime);
  void moveLeft(float fElapsedTime);
  void moveRight(float fElapsedTime);

  void draw();

private:
  olc::vi2d prevSprite = {4, 0};
  olc::vi2d vBlockSize = {32, 48};

  static olc::Sprite *sprPlayer;
  static olc::Decal *decPlayer;
};

#endif
