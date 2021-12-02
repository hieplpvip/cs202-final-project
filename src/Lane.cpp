#include "Lane.h"

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
