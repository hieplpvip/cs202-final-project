#include "Player.h"

olc::Sprite *Player::sprPlayer = nullptr;
olc::Decal *Player::decPlayer = nullptr;

bool Player::loadData() {
  sprPlayer = new olc::Sprite("assets/graphics/Player.png");
  if (sprPlayer == nullptr) {
    return false;
  }

  decPlayer = new olc::Decal(sprPlayer);
  if (decPlayer == nullptr) {
    return false;
  }

  return true;
}

void Player::unloadData() {
  if (decPlayer) {
    delete decPlayer;
    decPlayer = nullptr;
  }
  if (sprPlayer) {
    delete sprPlayer;
    sprPlayer = nullptr;
  }
}

Player::Player() {
  size = {12, 16};
  speed = 50;
}

void Player::moveUp(float fElapsedTime) {
  pos.y -= fElapsedTime * speed;
  if (pos.y < 0) {
    pos.y = 0;
  }
}

void Player::moveDown(float fElapsedTime) {
  pos.y += fElapsedTime * speed;
  if (pos.y > (float)pge->ScreenHeight() - size.y) {
    pos.y = (float)pge->ScreenHeight() - size.y;
  }
}

void Player::moveLeft(float fElapsedTime) {
  pos.x -= fElapsedTime * speed;
  if (pos.x < 0) {
    pos.x = 0;
  }
}

void Player::moveRight(float fElapsedTime) {
  pos.x += fElapsedTime * speed;
  if (pos.x > (float)pge->ScreenWidth() - size.x) {
    pos.x = (float)pge->ScreenWidth() - size.x;
  }
}

void Player::draw() {
  pge->DrawPartialDecal(pos, decPlayer, olc::vf2d(4, 0) * vBlockSize, vBlockSize, {0.5f, 0.5f});
}
