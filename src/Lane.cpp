#include "Lane.h"

Lane::Lane(olc::vf2d pos, int direction, float timeBetweenObstacles, float obstacleSpeed, int seed)
{
	this->pos = pos;
	this->direction = direction;
	this->timeBetweenObsstacles = timeBetweenObsstacles;
	this->obstacleSpeed = obstacleSpeed;
	this->setSeed(seed);
	this->timeAccumulator = 0;

	//Obstacle*
}

void Lane::setSeed(long long seed) {
  rnd.setSeed(seed);
}

void Lane::update(float fElapsedTime) {
  // TODO: check for out of window

  // TODO: generate new obstacles

  // TODO: generate coins
}

void Lane::draw(olc::PixelGameEngine* pge) {
  // TODO
}

bool Lane::checkCollision(Player* player) {
  // TODO
}

void Lane::checkCoin(Player* player, int& coinEaten) {
  // TODO
}
