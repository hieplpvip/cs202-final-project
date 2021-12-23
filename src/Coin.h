#ifndef COIN_H
#define COIN_H

#include "Entity.h"

class Coin : public Entity {
private:
  olc::Sprite* sprite = nullptr;

public:
  void draw(olc::PixelGameEngine* pge);
};

#endif
