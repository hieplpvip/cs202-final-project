#include "TrafficLight.h"
#include "Logging.h"

olc::Sprite *TrafficLight::sprGreen = nullptr;
olc::Sprite *TrafficLight::sprYellow = nullptr;
olc::Sprite *TrafficLight::sprRed = nullptr;
olc::Decal *TrafficLight::decGreen = nullptr;
olc::Decal *TrafficLight::decYellow = nullptr;
olc::Decal *TrafficLight::decRed = nullptr;

bool TrafficLight::loadData() {
  sprGreen = new olc::Sprite("assets/graphics/Traffic Green.png");
  sprYellow = new olc::Sprite("assets/graphics/Traffic Yellow.png");
  sprRed = new olc::Sprite("assets/graphics/Traffic Red.png");
  if (sprGreen == nullptr || sprYellow == nullptr || sprRed == nullptr) {
    return false;
  }

  decGreen = new olc::Decal(sprGreen);
  decYellow = new olc::Decal(sprYellow);
  decRed = new olc::Decal(sprRed);
  if (decGreen == nullptr || decYellow == nullptr || decRed == nullptr) {
    return false;
  }

  return true;
}

void TrafficLight::unloadData() {
  if (decGreen) {
    delete decGreen;
    decGreen = nullptr;
  }
  if (decYellow) {
    delete decYellow;
    decYellow = nullptr;
  }
  if (decRed) {
    delete decRed;
    decRed = nullptr;
  }
  if (sprGreen) {
    delete sprGreen;
    sprGreen = nullptr;
  }
  if (sprYellow) {
    delete sprYellow;
    sprYellow = nullptr;
  }
  if (sprRed) {
    delete sprRed;
    sprRed = nullptr;
  }
}

TrafficLight::TrafficLight() {
  scale = 0.2f;
  timeAccumulator = 0;
  currentColor = GREEN;
  pos = {(float)pge->ScreenWidth() - (float)sprGreen->width * scale - 2.0f, 2.0f};
}

void TrafficLight::reset() {
  Logging::debug("[TrafficLight::reset] Resetting traffic light\n");
  timeAccumulator = 0;
  currentColor = GREEN;
}

void TrafficLight::update(float fElapsedTime) {
  timeAccumulator += fElapsedTime;
  if (currentColor == GREEN && timeAccumulator >= 7.0f) {
    Logging::debug("[TrafficLight::update] Traffic light turns yellow\n");
    currentColor = YELLOW;
    timeAccumulator = 0;
  }

  else if (currentColor == YELLOW && timeAccumulator >= 3.0f) {
    Logging::debug("[TrafficLight::update] Traffic light turns red\n");
    currentColor = RED;
    timeAccumulator = 0;
  }

  else if (currentColor == RED && timeAccumulator >= 3.0f) {
    Logging::debug("[TrafficLight::update] Traffic light turns green\n");
    currentColor = GREEN;
    timeAccumulator = 0;
  }
}

void TrafficLight::draw() {
  pge->SetPixelMode(olc::Pixel::MASK);
  if (currentColor == GREEN) {
    // draw green light
    pge->DrawDecal(pos, decGreen, {scale, scale});
  } else if (currentColor == YELLOW) {
    // draw yellow light
    pge->DrawDecal(pos, decYellow, {scale, scale});
  } else {
    // draw red light
    pge->DrawDecal(pos, decRed, {scale, scale});
  }
  pge->SetPixelMode(olc::Pixel::NORMAL);
}

TrafficLightColor TrafficLight::getColor() const {
  return currentColor;
}
