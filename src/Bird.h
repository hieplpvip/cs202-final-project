#ifndef BIRD_H
#define BIRD_H

#include "Obstacle.h"

class Bird : public Obstacle {
	void draw(olc::PixelGameEngine* pge);
	void playCollisionSound();
};


#endif
