#include "Level.h"

void Level::setSeed(long long seed) {
  rnd.setSeed(seed);
}

void Level::update(float fElapsedTime) {
  // TODO
}

void Level::draw(olc::PixelGameEngine* pge) {
  // TODO
}

bool Level::checkCollision(Player* player) {
  // TODO
}

void Level::checkCoin(Player* player, int& coinEaten) {
  // TODO
}

bool Level::isComplete(Player* player) {
  // TODO
}
