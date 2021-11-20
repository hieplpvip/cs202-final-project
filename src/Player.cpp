#include "Player.h"

Player::Player() {
  pos = {0, 0};
  size = {10, 10};
  speed = 50;
}

void Player::moveUp(float fElapsedTime) {
  pos.y -= fElapsedTime * speed;
}

void Player::moveDown(float fElapsedTime) {
  pos.y += fElapsedTime * speed;
}

void Player::moveLeft(float fElapsedTime) {
  pos.x -= fElapsedTime * speed;
}

void Player::moveRight(float fElapsedTime) {
  pos.x += fElapsedTime * speed;
}

void Player::draw(olc::PixelGameEngine* pge) {
  pge->FillRect(pos, size, olc::YELLOW);
}
