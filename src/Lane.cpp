#include "Lane.h"

olc::Sprite* Lane::spr = nullptr;

bool Lane::loadData() {
  spr = new olc::Sprite("assets/graphics/Lane 3.png");
  return spr != nullptr;
}

void Lane::unloadData() {
  if (spr) {
    delete spr;
    spr = nullptr;
  }
}

Lane::Lane(olc::vf2d pos, int direction, float timeBetweenObstacles, float obstacleSpeed, long long seed) {
  this->pos = pos;
  this->direction = direction;
  this->timeBetweenObstacles = timeBetweenObstacles;
  this->obstacleSpeed = obstacleSpeed;
  this->setSeed(seed);
  this->timeAccumulator = 0;

  // Initialize the first obstacle
  Obstacle* obstacle = generateObstacle();
  obstacle->setDirection(direction == 0 ? LEFT : RIGHT);
  obstacle->setPosition({(float)rnd.next((double)(pge->ScreenWidth() - obstacle->getSize().x)), pos.y + (20 - obstacle->getSize().y) / 2.0f});
  obstacle->setSpeed(obstacleSpeed);
  obstacles.push_back(obstacle);

  // Initialize the 

  coin = new Coin();
  coin->setPosition({(float)rnd.next(pge->ScreenWidth() * 0.2f, pge->ScreenWidth() * 0.8f), pos.y + (20 - coin->getSize().y) / 2.0f});
}

Lane::~Lane() {
  for (auto& obstacle : obstacles) {
    delete obstacle;
  }
  obstacles.clear();
  if (coin != nullptr) {
    delete coin;
    coin = nullptr;
  }
}

void Lane::setSeed(long long seed) {
  rnd.setSeed(seed);
}

void Lane::update(float fElapsedTime) {
  timeAccumulator += fElapsedTime;

  // Move obstacles
  for (auto& obstacle : obstacles) {
    obstacle->move(fElapsedTime);
  }

  // Remove obstacles that have moved out of window
  for (auto it = obstacles.begin(); it != obstacles.end();) {
    if ((*it)->getPosition().x > pge->ScreenWidth()) {
      Logging::debug("[Lane::update] Removing out-of-window obstacle\n");
      delete *it;
      it = obstacles.erase(it);
    } else {
      ++it;
    }
  }

  if (timeAccumulator >= timeBetweenObstacles) {
    // Generate new obstacles with probability 1/3
    timeAccumulator = 0;
    if (rnd.next(3) == 0) {
      Logging::debug("[Lane::update] Generating new obstacle\n");
      Obstacle* obstacle = generateObstacle();
      obstacle->setDirection(direction == 0 ? LEFT : RIGHT);
      obstacle->setPosition({direction == 0 ? pge->ScreenWidth() : -obstacle->getSize().x, pos.y + (20 - obstacle->getSize().y) / 2.0f});
      obstacle->setSpeed(obstacleSpeed);
      obstacles.push_back(obstacle);
    }
  }
}

void Lane::drawBackground() {
  // Draw the road
  //pge->SetPixelMode(olc::Pixel::MASK);
  pge->DrawSprite(pos, spr);
  pge->SetPixelMode(olc::Pixel::NORMAL);
}

void Lane::drawObjects() {
  // Draw the coin (if not eaten yet)
  if (coin != nullptr) {
    coin->draw();
  }

  // Draw the obstacles
  for (auto& obstacle : obstacles) {
    obstacle->draw();
  }
}

Obstacle* Lane::generateObstacle() {
  int type = rnd.next(4);
  if (type == 0) {
    return new Bird();
  } else if (type == 1) {
    return new Car();
  } else if (type == 2) {
    return new Elephant();
  } else {
    return new Truck();
  }
}

bool Lane::checkCollision(Player* player , bool sound) {
  for (auto& obstacle : obstacles) {
    if (obstacle->intersectWithEntity(player)) {
      if(sound)
         obstacle->playCollisionSound();
      return true;
    }
  }
  return false;
}

void Lane::checkCoin(Player* player, int& coinEaten) {
  if (coin != nullptr && coin->intersectWithEntity(player)) {
    Logging::info("Coin eaten\n");
    coinEaten += 1;
    delete coin;
    coin = nullptr;
  }
}
