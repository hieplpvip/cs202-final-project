#include "Coin.h"

void Coin::draw() {
  sprite = new olc::Sprite("assets/graphics/Coin S.png");
  if (sprite) {
    pge->DrawSprite(pos, sprite);
  }
}
