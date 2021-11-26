#include"Truck.h"

void Truck::draw(olc::PixelGameEngine* pge) {
	if (direction == LEFT)
		sprite = new olc::Sprite("assets/graphics/Truck_left.png");
	else sprite == new olc::Sprite("assets/graphics/Truck_right.png");
	Obstacle::draw(pge);
}
