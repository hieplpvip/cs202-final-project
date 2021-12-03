#ifndef LANE_H
#define LANE_H

#include <vector>
#include "Coin.h"
#include "Obstacle.h"
#include "Player.h"
#include "Random.h"
#include "olcPixelGameEngine.h"

class Lane {
public:
	Lane(olc::vf2d pos, int direction, float timeBetweenObstacles, float obstacleSpeed, int seed);
	~Lane();
  void setSeed(long long seed);
  void update(float fElapsedTime);
  void draw(olc::PixelGameEngine* pge);

  Obstacle* generateObstacle();

  bool checkCollision(Player* player);
  void checkCoin(Player* player, int& coinEaten);

private:
	olc::vf2d pos;
	int direction;
	float timeBetweenObstacles;
	float obstacleSpeed;
	float timeAccumulator;

  Random rnd;
  std::vector<Obstacle*> obstacles;
  //std::vector<Coin*> coins;
  Coin* coin;
};

#endif
