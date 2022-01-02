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
  speed = 70;
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
  int dx = 0, dy = 0;
  if (pge->GetKey(olc::LEFT).bHeld) --dx;
  if (pge->GetKey(olc::RIGHT).bHeld) ++dx;
  if (pge->GetKey(olc::UP).bHeld) --dy;
  if (pge->GetKey(olc::DOWN).bHeld) ++dy;
  if (dy == -1) {
    if (dx == -1) {
      prevSprite = {1, 0};
    } else if (dx == 0) {
      prevSprite = {4, 0};
    } else {
      prevSprite = {7, 0};
    }
  } else if (dy == +1) {
    if (dx == -1) {
      prevSprite = {1, 4};
    } else if (dx == 0) {
      prevSprite = {4, 4};
    } else {
      prevSprite = {7, 4};
    }
  } else {
    if (dx == -1) {
      prevSprite = {2, 2};
    } else if (dx == +1) {
      prevSprite = {7, 2};
    }
  }
  pge->DrawPartialDecal(pos, decPlayer, prevSprite * vBlockSize, vBlockSize, {0.5f, 0.5f});
}
