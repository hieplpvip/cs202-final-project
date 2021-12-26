#include "Level.h"

Level::Level(float timeBetweenObstacles, float obstacleSpeed, int numberOfLanes, Player* player, int seed) {
  this->timeBetweenObstacles = timeBetweenObstacles;
  this->obstacleSpeed = obstacleSpeed;
  this->numberOfLanes = numberOfLanes;
  this->player = player;
  this->setSeed(seed);

  // Initialize the lanes
  topLanePos = (pge->ScreenHeight() - 20 * numberOfLanes) / 2;
  bottomLanePos = topLanePos + 20 * numberOfLanes;
  for (int i = 0; i < numberOfLanes; i++) {
    Lane* lane = new Lane({0, (float)(topLanePos + i * 20)}, rnd.next(2), timeBetweenObstacles, obstacleSpeed, rnd.next(1000000000));
    lanes.push_back(lane);
  }

  // Set initial position of the player
  player->setPosition({pge->ScreenWidth() / 2.0f, (float)bottomLanePos});
}

Level::~Level() {
  for (auto& lane : lanes) {
    delete lane;
  }
  lanes.clear();
}

void Level::setSeed(long long seed) {
  rnd.setSeed(seed);
}

void Level::update(float fElapsedTime) {
  for (auto& lane : lanes) {
    lane->update(fElapsedTime);
  }
}

void Level::draw() {
  // TODO: draw pavements
  /*
  sprite = new olc::Sprite("assets/graphics/Pavement.png");
  if (sprite)
    pge->DrawSprite(pos, sprite);
  */

  // Draw the lanes
  for (auto& lane : lanes) {
    lane->drawBackground();
  }
  for (auto& lane : lanes) {
    lane->drawObjects();
  }
}

bool Level::checkCollision() {
  for (auto& lane : lanes) {
    if (lane->checkCollision(player)) {
      return true;
    }
  }
  return false;
}

void Level::checkCoin(int& coinEaten) {
  for (auto& lane : lanes) {
    lane->checkCoin(player, coinEaten);
  }
}

bool Level::isComplete() {
  return (int)(player->getPosition().y + player->getSize().y) < topLanePos;
}
