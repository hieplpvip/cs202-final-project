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
  void setSeed(long long seed);
  void update(float fElapsedTime);
  void draw(olc::PixelGameEngine* pge);

  bool checkCollision(Player* player);
  void checkCoin(Player* player, int& coinEaten);

private:
  Random rnd;
  std::vector<Obstacle*> obstacles;
  std::vector<Coin*> coins;
};

#endif
