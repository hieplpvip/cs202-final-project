#ifndef COIN_H
#define COIN_H

#include "Entity.h"

class Coin : public Entity {
public:
  void draw(olc::PixelGameEngine* pge);
};

#endif
