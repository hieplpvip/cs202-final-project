#include "Obstacle.h"

Obstacle::Obstacle()
{
	pos = { 0, 0 };
	size = { 20, 20 };
}

void Obstacle::setDirection(DIRECTION direction) {
  this->direction = direction;
}

void Obstacle::move(float fElapsedTime) {
  // TODO
}

void Obstacle::draw(olc::PixelGameEngine* pge) {
	if (sprite)
		pge->DrawSprite(pos, sprite);
}
