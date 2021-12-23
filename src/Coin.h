#ifndef COIN_H
#define COIN_H

#include "Entity.h"

class Coin : public Entity {
public:
  static bool loadData();
  static void unloadData();

  void draw();

private:
  static olc::Sprite* spr;
};

#endif
