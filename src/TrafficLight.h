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

  static bool loadData();
  static void unloadData();

  void reset();
  void update(float fElapsedTime);
  void draw();

  TrafficLightColor getColor() const;

private:
  static olc::Sprite *sprGreen;
  static olc::Sprite *sprYellow;
  static olc::Sprite *sprRed;
  static olc::Decal *decGreen;
  static olc::Decal *decYellow;
  static olc::Decal *decRed;

  float scale;
  float timeAccumulator;
  TrafficLightColor currentColor;
};

#endif
