#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Lane.h"
#include "Player.h"
#include "Random.h"
#include "olcPixelGameEngine.h"

extern olc::PixelGameEngine* pge;

class Level {
public:
	Level(float timeBetweenObstacles, float obstacleSpeed, int numberOfLanes, Player* player, int seed);
	~Level();
  void setSeed(long long seed);
  void update(float fElapsedTime);
  void draw(olc::PixelGameEngine* pge);

  bool checkCollision(Player* player);
  void checkCoin(Player* player, int& coinEaten);

  bool isComplete(Player* player);

private:
	float timeBetweenObstacles;
	float obstacleSpeed;
	int numberOfLanes;
	int topLanePos;
	int bottomLanePos;
	
	Random rnd;
	Player* player;
	std::vector<Lane*> lanes;
	olc::Sprite* sprite = nullptr;
};

#endif
