#ifndef TRUCK_H
#define TRUCK_H

#include "Obstacle.h"

class Truck : public Obstacle {
	void draw(olc::PixelGameEngine* pge);
	void playCollisionSound();
};

#endif
