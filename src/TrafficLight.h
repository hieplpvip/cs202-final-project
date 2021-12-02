#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "Entity.h"

enum TrafficLightColor {
  RED,
  YELLOW,
  GREEN,
};

class TrafficLight : public Entity {
public:
  TrafficLight();

  void update(float fElapsedTime);
  void draw(olc::PixelGameEngine* pge);

  TrafficLightColor getColor() const;

private:
	olc::Sprite* sprite = nullptr;
  float timeAccumulator;
  TrafficLightColor currentColor;
};

#endif
