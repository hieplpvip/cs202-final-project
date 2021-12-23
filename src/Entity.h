#ifndef ENTITY_H
#define ENTITY_H

#include "olcPixelGameEngine.h"

extern olc::PixelGameEngine* pge;

class Entity {
public:
  olc::vf2d getPosition() const;
  void setPosition(olc::vf2d pos);

  olc::vf2d getSize() const;
  void setSize(olc::vf2d size);

  float getSpeed() const;
  void setSpeed(float speed);

  bool intersectWithEntity(Entity* other) const;

  virtual void draw() = 0;

protected:
  olc::vf2d pos;
  olc::vf2d size;
  float speed;
};

#endif
