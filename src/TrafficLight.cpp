#include "TrafficLight.h"
#include "Logging.h"

olc::Sprite *TrafficLight::sprGreen = nullptr;
olc::Sprite *TrafficLight::sprYellow = nullptr;
olc::Sprite *TrafficLight::sprRed = nullptr;

bool TrafficLight::loadData() {
  sprGreen = new olc::Sprite("assets/graphics/Traffic Green.png");
  sprYellow = new olc::Sprite("assets/graphics/Traffic Yellow.png");
  sprRed = new olc::Sprite("assets/graphics/Traffic Red.png");

  return sprGreen != nullptr && sprYellow != nullptr && sprRed != nullptr;
}

void TrafficLight::unloadData() {
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
  timeAccumulator = 0;
  currentColor = GREEN;
}

void TrafficLight::reset() {
  Logging::debug("[TrafficLight::reset] Resetting traffic light");
  timeAccumulator = 0;
  currentColor = GREEN;
}

void TrafficLight::update(float fElapsedTime) {
  timeAccumulator += fElapsedTime;
  if (currentColor == GREEN && timeAccumulator >= 7.0f) {
    Logging::debug("[TrafficLight::update] Traffic light turns yellow");
    currentColor = YELLOW;
    timeAccumulator = 0;
  }

  else if (currentColor == YELLOW && timeAccumulator >= 3.0f) {
    Logging::debug("[TrafficLight::update] Traffic light turns red");
    currentColor = RED;
    timeAccumulator = 0;
  }

  else if (currentColor == RED && timeAccumulator >= 3.0f) {
    Logging::debug("[TrafficLight::update] Traffic light turns green");
    currentColor = GREEN;
    timeAccumulator = 0;
  }
}

void TrafficLight::draw() {
  pge->SetPixelMode(olc::Pixel::MASK);
  if (currentColor == GREEN) {
    // draw green light
    pge->DrawSprite(pos, sprGreen);
  } else if (currentColor == YELLOW) {
    // draw yellow light
    pge->DrawSprite(pos, sprYellow);
  } else {
    // draw red light
    pge->DrawSprite(pos, sprRed);
  }
  pge->SetPixelMode(olc::Pixel::NORMAL);
}

TrafficLightColor TrafficLight::getColor() const {
  return currentColor;
}
