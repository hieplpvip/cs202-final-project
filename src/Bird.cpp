#include"Bird.h"

void Bird::draw(olc::PixelGameEngine* pge) {
	if (direction == LEFT)
		sprite = new olc::Sprite("assets/graphics/Bird_fly_left.png");
	//sprite = new olc::Sprite("assets/graphics/Bird_walk_left.png");
	else sprite == new olc::Sprite("assets/graphics/Bird_fly_right.png");
	//sprite = new olc::Sprite("assets/graphics/Bird_fly_left.png");
	Obstacle::draw(pge);
}

void Bird::playCollisionSound() {
	snd = olc::SOUND::LoadAudioSample("assets/sound/animal_collision.wav");
	Obstacle::playCollisionSound();
}