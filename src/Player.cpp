#include "Player.h"

Player::Player() {
  pos = {0, 0};
  size = {10, 10};
  speed = 50;
  // speed=70;
}

void Player::moveUp(float fElapsedTime) {
  pos.y -= fElapsedTime * speed;
  //if (pos.y<0) pos.y=0;
}

void Player::moveDown(float fElapsedTime) {
  pos.y += fElapsedTime * speed;
  /*if (pos.y > (float)pge->ScreenHeight() - size.y)
	  pos.y = (float)pge->ScreenHeight() - size.y;*/
}

void Player::moveLeft(float fElapsedTime) {
  pos.x -= fElapsedTime * speed;
  //if (pos.x<0) pos.x=0;
}

void Player::moveRight(float fElapsedTime) {
  pos.x += fElapsedTime * speed;
  /*if (pos.x > (float)pge->ScreenHeight() - size.x)
	  pos.x = (float)pge->ScreenHeight() - size.x;*/
}

void Player::draw(olc::PixelGameEngine* pge) {
  pge->FillRect(pos, size, olc::YELLOW);
}
