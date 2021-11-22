#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Lane.h"
#include "Player.h"
#include "Random.h"
#include "olcPixelGameEngine.h"

class Level {
public:
  void setSeed(long long seed);
  void update(float fElapsedTime);
  void draw(olc::PixelGameEngine* pge);

  bool checkCollision(Player* player);
  void checkCoin(Player* player, int& coinEaten);

  bool isComplete(Player* player);

private:
  Random rnd;
  std::vector<Lane*> lanes;
};

#endif
