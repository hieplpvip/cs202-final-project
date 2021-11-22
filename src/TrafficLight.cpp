#include "TrafficLight.h"

TrafficLight::TrafficLight() {
  timeAccumulator = 0;
  currentColor = GREEN;
}

void TrafficLight::update(float fElapsedTime) {
  // TODO
}

void TrafficLight::draw(olc::PixelGameEngine* pge) {
  // TODO
}

TrafficLightColor TrafficLight::getColor() const {
  return currentColor;
}
