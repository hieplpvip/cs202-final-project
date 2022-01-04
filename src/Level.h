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
  Level(float timeBetweenObstacles, float obstacleSpeed, int numberOfLanes, Player* player, long long seed);
  ~Level();

  static bool loadData();
  static void unloadData();

  void setSeed(long long seed);
  void update(float fElapsedTime);
  void draw();

  bool checkCollision(bool sound);
  void checkCoin(int& coinEaten);

  bool isComplete();

private:
  float timeBetweenObstacles;
  float obstacleSpeed;
  int numberOfLanes;
  int topLanePos;
  int bottomLanePos;

  Random rnd;
  Player* player;
  std::vector<Lane*> lanes;

  static olc::Sprite* sprGrass;
};

#endif
