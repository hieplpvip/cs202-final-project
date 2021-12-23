#include "Coin.h"

olc::Sprite *Coin::spr = nullptr;

bool Coin::loadData() {
  spr = new olc::Sprite("assets/graphics/Coin S.png");
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
