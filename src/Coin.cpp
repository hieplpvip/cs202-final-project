#include "Coin.h"

olc::Sprite *Coin::spr = nullptr;

Coin::Coin() {
  size = {(float)spr->width, (float)spr->height};
}

bool Coin::loadData() {
  spr = new olc::Sprite("assets/graphics/Coin S (2) copy.png");
  return spr != nullptr;
}

void Coin::unloadData() {
  if (spr) {
    delete spr;
    spr = nullptr;
  }
}

void Coin::draw() {
  pge->SetPixelMode(olc::Pixel::MASK);
  pge->DrawSprite(pos, spr);
  pge->SetPixelMode(olc::Pixel::NORMAL);
}
