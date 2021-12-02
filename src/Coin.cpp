#include "Coin.h"

void Coin::draw(olc::PixelGameEngine* pge) {
	sprite = new olc::Sprite("assets/graphics/Coin.png");
	if (sprite)
		pge->DrawSprite(pos, sprite);
}
