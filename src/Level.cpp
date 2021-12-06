#include "Level.h"

Level::Level(float timeBetweenObstacles, float obstacleSpeed, int numberOfLanes, Player* player, int seed)
{
	this->timeBetweenObstacles = timeBetweenObstacles;
	this->obstacleSpeed = obstacleSpeed;
	this->numberOfLanes = numberOfLanes;
	this->player = player;
	this->setSeed(seed);

	topLanePos = (pge->ScreenHeight() - 30 * numberOfLanes) / 2;
	bottomLanePos = topLanePos + 30 * numberOfLanes;
	for (int i = 0; i < numberOfLanes; i++) {
		Lane* lane = new Lane({ 0, (float)(topLanePos + i * 30) }, rnd.next(2), timeBetweenObstacles, obstacleSpeed, rnd.next(1000000000));
		lanes.push_back(lane);
	}

	player->setPosition({ pge->ScreenWidth() / 2.0f, bottomLanePos });
}

Level::~Level()
{
	for (auto& lane : lanes)
		delete lane;
	lanes.clear();
}

void Level::setSeed(long long seed) {
  rnd.setSeed(seed);
}

void Level::update(float fElapsedTime) {
  // TODO
	for (auto& lane : lanes)
		lane->update(fElapsedTime);
	
}

void Level::draw(olc::PixelGameEngine* pge) {
  // TODO
	// draw pavements
	/*sprite = new olc::Sprite("assets/graphics/Pavement.png");
	if (sprite)
		pge->DrawSprite(pos, sprite);*/

	// draw lane
	for (auto& lane : lanes)
		lane->draw(pge);

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
