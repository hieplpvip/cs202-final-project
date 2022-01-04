#ifndef LANE_H
#define LANE_H

#include <vector>
#include "Bird.h"
#include "Car.h"
#include "Coin.h"
#include "Elephant.h"
#include "Logging.h"
#include "Obstacle.h"
#include "Player.h"
#include "Random.h"
#include "Truck.h"
#include "olcPixelGameEngine.h"

extern olc::PixelGameEngine* pge;

class Lane {
public:
  Lane(olc::vf2d pos, int direction, float timeBetweenObstacles, float obstacleSpeed, long long seed);
  ~Lane();

  static bool loadData();
  static void unloadData();

  void setSeed(long long seed);
  void update(float fElapsedTime);
  void drawBackground();
  void drawObjects();

  Obstacle* generateObstacle();

  bool checkCollision(Player* player , bool sound);
  void checkCoin(Player* player, int& coinEaten);

private:
  olc::vf2d pos;
  int direction;
  float timeBetweenObstacles;
  float obstacleSpeed;
  float timeAccumulator;

  Random rnd;
  std::vector<Obstacle*> obstacles;
  // std::vector<Coin*> coins;
  Coin* coin;

  static olc::Sprite* spr;
};

#endif
