#include "Level.h"

olc::Sprite* Level::sprGrass = nullptr;

bool Level::loadData() {
  sprGrass = new olc::Sprite("assets/graphics/Grass.png");
  return (sprGrass != nullptr);
}

void Level::unloadData() {
  if (sprGrass) {
    delete sprGrass;
    sprGrass = nullptr;
  }
}

Level::Level(float timeBetweenObstacles, float obstacleSpeed, int numberOfLanes, Player* player, long long seed) {
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
  // Draw pavements
  pge->DrawSprite({0, 0}, sprGrass);

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
